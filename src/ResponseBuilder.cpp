#include <string>
#include <iostream>
#include <fstream>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <sstream>
#include <vector>

#include "ResponseBuilder.hpp"
#include "Conf.hpp"
#include "common/utils.hpp"
#include "common/messages.hpp"
#include "tools/ziafilesystem.hpp"


ResponseBuilder::ResponseBuilder(Transaction& trans, std::istream& is, std::ostream& os)
{
  this->methods["GET"] = &ResponseBuilder::doGet;
  this->methods["HEAD"] = &ResponseBuilder::doHead;
  this->methods["POST"] = &ResponseBuilder::doGet;
  this->trans = &trans;
  this->is = &(is);
  this->os = &(os);
}

void		ResponseBuilder::buildResponse(void)
{
  if ((this->checkHttpVersion() == false) ||
      (this->checkImplementedMethod() == false))
    return;
  method_ptr	met = this->methods[this->trans->getRequest().getMethod()];
  (this->*met)();
  return ;
}

bool		ResponseBuilder::checkHttpVersion(void)
{
  if (this->trans->getRequest().getVersion() != "HTTP/1.1" && this->trans->getRequest().getVersion() != "HTTP/1.0")
    {
      this->trans->getResponse().setStatusCode(505);
      this->trans->getResponse().setStatusMessage(MESSAGE_505);
      (*this->os) << createBody(MESSAGE_505);
      return false;
    }
  return true;
}

bool		ResponseBuilder::checkImplementedMethod(void)
{
  if (this->methods.find(this->trans->getRequest().getMethod()) == this->methods.end())
    {
      this->trans->getResponse().setStatusCode(501);
      this->trans->getResponse().setStatusMessage(MESSAGE_501);
      (*this->os) << createBody(MESSAGE_501);
      return false;
    }
  return true;
}

bool		ResponseBuilder::checkPermission(const std::string& filename)
{
  ZiaFileSystem io;

  if (io.access(filename.data(), F_OK) != 0)
    {
      this->notFound(filename);
      return false;
    }
  else if (io.access(filename.data(), R_OK) != 0)
    {
      this->permissionDenied(filename);
      return false;
    }
  return true;
}

void		ResponseBuilder::notFound(const std::string& filename)
{
  this->trans->getResponse().setStatusCode(404);
  this->trans->getResponse().setStatusMessage(MESSAGE_404);
  std::string msg = std::string(MESSAGE_404) + ": ";
  msg += filename;
  (*this->os) << createBody(msg.data());
}

void		ResponseBuilder::permissionDenied(const std::string& filename)
{
  this->trans->getResponse().setStatusCode(403);
  this->trans->getResponse().setStatusMessage(MESSAGE_403);
  std::string msg = std::string(MESSAGE_403) + ": ";
  msg += filename;
  (*this->os) << createBody(msg.data());
}

void		ResponseBuilder::movedPermanently(const std::string& filename)
{
  this->trans->getResponse().setStatusCode(301);
  this->trans->getResponse().setStatusMessage(MESSAGE_301);
  std::string msg = std::string(MESSAGE_301) + ": ";
  msg += filename;
  this->trans->getResponse().getHeaders().setValue("LOCATION", filename);
  (*this->os) << createBody(msg.data());
}

std::string	ResponseBuilder::createBody(const char *str) const
{
  std::string ret;
  ret += std::string(BEGIN_MESSAGE);
  ret += std::string(str);
  ret += std::string(MIDDLE_MESSAGE);
  ret += std::string(str);
  ret += std::string(END_MESSAGE);
  return ret;
}


void		ResponseBuilder::directoryListing(const std::string& directory_name)
{
  // Content-Type: text/html; charset=iso-8859-1

  this->trans->getResponse().getHeaders().setValue("CONTENT-TYPE", "text/html; charset=utf-8");
  this->trans->getResponse().setStatusCode(200);
  this->trans->getResponse().setStatusMessage("OK");
  std::string str(BEGIN_MESSAGE);
  str += "Index of ";
  std::cout << Conf::Inst().get("documentroot", "/www", inttostr(this->trans->getClientEndPoint().getPort()), this->trans->getRequest().getHeaders()["HOST"]) << std::endl;
  str += urldecode(directory_name.substr(Conf::Inst().get("documentroot", "/www", inttostr(this->trans->getClientEndPoint().getPort()), this->trans->getRequest().getHeaders()["HOST"]).size()));
  str += MIDDLE_MESSAGE;
  str += "<h1>Index of ";
  str += directory_name.substr(Conf::Inst().get("documentroot", "/www", inttostr(this->trans->getClientEndPoint().getPort()), this->trans->getRequest().getHeaders()["HOST"]).size());
  str += "</h1><table><tr><th>Name</th></tr>";
  std::vector<std::string>	list;
  ZiaFileSystem io;
  io.directoryList(directory_name, list);
  std::vector<std::string>::iterator it = list.begin();
  for (;it != list.end(); ++it)
    {
      str += "<tr><td><a href='";
      str += *it;
      str += "'>";
      str += *it;
      str += "</a></td></tr>";
    }
  str += "</table>";
  str += END_MESSAGE;
  (*this->os) << str;
}

void			ResponseBuilder::sendFile(const std::string& filename)
{
  // TODO put a header with the mime type
  this->trans->getResponse().setStatusCode(200);
  this->trans->getResponse().setStatusMessage("OK");

  std::ifstream is(filename.data(), std::ios::binary);
  std::string content;
  while (!is.eof())
    content += is.get();
  (*this->os) << content;
}

const std::string	ResponseBuilder::tryDefaultFile(const std::string& directory_name)
{
  std::string index_filenames = Conf::Inst().get("directoryindex", "index.html");
  std::vector<std::string>* names = splitString(index_filenames, " ");
  std::vector<std::string>::iterator it;
  ZiaFileSystem io;
  for (it = names->begin(); it != names->end(); ++it)
    {
      std::string file = directory_name;
      file += (*it);
	  if (io.access(file.data(), F_OK) == 0)
  	return file;
    }
  delete names;
  return "";
}

bool		ResponseBuilder::doGet()
{
  std::string URI = this->trans->getRequest().getPath();
  if (this->checkPermission(URI) == false)
    return false;
  std::string filename;
  if (isdir(URI.data()) != 0)
    {
      if (URI[URI.size()-1] != '/')
	{
	  std::string newuri = URI.substr(Conf::Inst().get("documentroot", "/www", inttostr(this->trans->getClientEndPoint().getPort()), this->trans->getRequest().getHeaders()["HOST"]).size());
	  this->movedPermanently(newuri + "/");
	  return true;
	}
      // check index files
      else if ((filename = this->tryDefaultFile(URI)) == "")
	{ // There is no index.* in the directory, do the listing
	  this->directoryListing(URI);
	  return true;
	}
      else
	{
	  std::string newURI = filename.substr(Conf::Inst().get("documentroot", "/www", inttostr(this->trans->getClientEndPoint().getPort()), this->trans->getRequest().getHeaders()["HOST"]).size());
	  this->movedPermanently(newURI);
	  return true;
	}
    }
  else
    filename = URI;
  this->sendFile(filename);
  return true;
}

bool		ResponseBuilder::doHead()
{
  this->doGet();
  std::stringbuf empty_buf;
  this->os->rdbuf(&empty_buf);
  return true;
}
