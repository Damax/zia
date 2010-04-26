#include <iostream>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <vector>
#include <errno.h>
#include "common/utils.hpp"
#include "ModPhpHandler.hpp"

extern char** environ;

ModPhpHandler::ModPhpHandler(): _map(NULL), _file(NULL)
{ }

ModPhpHandler::~ModPhpHandler(void)
{ }


void	ModPhpHandler::setFile(std::ofstream *file)
{
    this->_file = file;
}

void	ModPhpHandler::configure(const zia::api::IConfig::ValueMap& map)
{
  this->_map = &map;
}

zia::api::handler::EHook    ModPhpHandler::getHook(void) const
{
  return (zia::api::handler::ResponseProcessor);
}

void	vectorToTab(std::vector<std::string>* v, char** tab)
{
  int i = 0;

  if (v == NULL)
    {
      std::vector<std::string>::iterator	argsIt = v->begin();
      for (; argsIt != v->end(); ++argsIt, ++i)
	{
	  tab[i] = const_cast<char*>(argsIt->data());
	  std::cout << "tab[" << i << "] = " << tab[i] << std::endl;
	}
    }
  tab[i] = NULL;
}


void	ModPhpHandler::inParent(int *p, std::ostream& os, std::istream& is)
{
  std::cout << "ModPhpHandler::inParent" << std::endl;
  if (dup2(p[0], 0) == -1)
    std::cerr << "dup2 failed in parent" << std::endl;

  //   close(p[1]);
  // TODO: envoyer le $_POST dans p[1]
  char buf2[4096];
  int read_size = 0;
    while (!is.eof())
      {
	is.read(buf2, 4096);
	if (is.eof())
	  read_size = is.gcount();
	else
	  read_size = 4096;
	if (read_size == 0)
	  continue ;
	if (write(p[1], buf2, read_size) == -1)
	  {
	    std::cerr << "Write fail in ModPhpHandler" << std::endl;
	    break ;
	  }
	// la t'ecris buf dans le pipe
      }



  
  char	buf[512];
  memset(buf, 0, 512);
  while (read(0, buf, 512) > 0)
    {
      os << buf;
      std::cout << buf << std::endl;
      memset(buf, 0, 512);
    }
}

void	ModPhpHandler::inChild(int *p,
			       const char *bin,
			       const std::string& uri, 
			       zia::api::http::ITransaction & transac,
			       std::istream& is)
{
  std::string argsstr, pipeSize, queryString, scriptFilename;
  
  if (uri.find("?") != std::string::npos)
    argsstr = uri.substr(uri.find("?") + 1);
  std::cout << "argsstr = " << argsstr << std::endl;
  if (dup2(p[1], 1) == -1)
    std::cerr << "dup2 failed in child" << std::endl;

  if (dup2(p[0], 0) == -1)
    std::cerr << "dup2 failed in child" << std::endl;

  char* tab[6];

  std::string lengh("CONTENT_LENGTH=");
  is.seekg(0, std::ios::end);
  int is_size = is.tellg();  tab[0] = const_cast<char*>((lengh + inttostr(is_size)).data());

  tab[1] = "REQUEST_METHOD=POST";
  tab[2] = "CONTENT_TYPE=application/x-www-form-urlencoded";
  
  queryString = "QUERY_STRING=" + argsstr;
  tab[3] = const_cast<char*>(queryString.data());
  
  scriptFilename = "SCRIPT_FILENAME=" + transac.getRequest().getPath();
  tab[4] = const_cast<char*>(scriptFilename.data());
  tab[5] = NULL;

  char* arg[2];
  arg[0] = const_cast<char*>(bin);
  arg[1] = NULL;
  if (execve(arg[0], arg, tab) == -1)
    {
      std::cerr << "Execvp failed " <<  std::endl;
      perror("execve");
    }
}


zia::api::handler::ECode    ModPhpHandler::operator()(zia::api::http::ITransaction & transac,
						      std::istream & is,
						      std::ostream & os)
{

  if (this->_map == NULL)
    return zia::api::handler::ServerError;
      
  const std::string&	path = transac.getRequest().getPath();
  int	p[2];
  if (pipe(p) == -1)
    {
      std::cerr << "Pipe failed" << std::endl;
      return  zia::api::handler::ServerError;
    }
  
  std::string extens;
  if (path.rfind(".") != std::string::npos)
    extens = path.substr(path.rfind(".") + 1);
  zia::api::IConfig::ValueMap* maptest = (zia::api::IConfig::ValueMap*)(_map);
  zia::api::IConfig::ValueMap::iterator it2 = maptest->begin();
  zia::api::IConfig::ValueMap::const_iterator conf = this->_map->begin();

  if (conf == this->_map->end())
    return zia::api::handler::Decline;

  std::multimap<std::string, std::string>::const_iterator it = conf->second.find(extens);

  for (;it != conf->second.end() && it->first != extens; ++it);
  if (it == conf->second.end())
    return zia::api::handler::Decline;

  int res = fork();
  if (res == -1)
    {
      std::cerr << "Fork failed" << std::endl;
      return zia::api::handler::ServerError;
    }
  if (res == 0)
    {
      inChild(p, it->second.data(),transac.getRequest().getUri(), transac, is);
      exit(-1);
    }
  else
    {
      int status;
      res = fork();
      if (res == -1)
	{
	  std::cerr << "Fork has failed" << std::endl;
	  return zia::api::handler::ServerError;
	}
      if (res == 0)
	{
	  inParent(p, os, is);
	  waitpid(res, &status, 0);
	  exit(status);
	}
      else
	{
	  waitpid(res, &status, 0);
 	  if (status == 0)
 	    return zia::api::handler::Ok;
	  //exit(status);
	} 
    }
  return zia::api::handler::ServerError;
}

