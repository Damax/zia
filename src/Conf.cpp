
#include <QFile>
#include <QString>
#include <QDomDocument>
#include <stdlib.h>

#include "Conf.hpp"
#include "common/utils.hpp"

Conf::Conf()
{
}

Conf::Conf(const Conf& ref):
  v_hosts(ref.v_hosts)
{
}

Conf::~Conf()
{}

Conf&	Conf::operator=(const Conf& ref)
{
  v_hosts = ref.v_hosts;
  return *this;
}

Conf&	Conf::Inst()
{
  if (pInstance == 0)
    {
      pInstance = new Conf();
    }
  return *(pInstance);
}

void	Conf::parseVirtualHost(QDomElement& parent)
{
  QDomElement node = parent.firstChildElement();
  std::map<std::string, std::string> dic;

  while (!node.isNull())
    {
      dic[strlowercase(node.tagName().toStdString())] = node.attribute("v").toStdString();
      node = node.nextSiblingElement();
    }
  if ((dic["port"] == "") || (atoi(dic["port"].data()) <= 0))
    {
      std::cerr << "Warning [CONFIG]: VirtualHost without a valid port" << std::endl;
      return ;
    }
  this->v_hosts.push_back(dic);
}

void	Conf::parseMainConf(QDomElement& parent)
{
  QDomElement node = parent.firstChildElement();
  while (!node.isNull())
    {
      this->default_conf[strlowercase(node.tagName().toStdString())] = node.attribute("v").toStdString();
      node = node.nextSiblingElement();
    }
}

void	Conf::parseModule(QDomElement& parent)
{
  if (parent.hasAttribute("name") == false)
    {
      std::cerr << "Module with no name, ignoring." << std::endl;
      return ;
    }
  QString name = parent.attribute("name");
  QDomElement node = parent.firstChildElement();
  QDomElement son;
  ModConf mod;

  while (!node.isNull())
    {
      son = node.firstChildElement();
      while (!son.isNull())
	{
	  mod.addEntry(node.tagName().toStdString(), strlowercase(son.tagName().toStdString()), son.attribute("v").toStdString());
	  son = son.nextSiblingElement();
	}
      node = node.nextSiblingElement();
    }
  this->mods[name.toStdString()] = mod;
}

void	Conf::deleteOldConf(void)
{
  this->default_conf.clear();
  this->v_hosts.clear();
  this->mods.clear();
}

void	Conf::loadConfig(const std::string& filename)
{
  this->deleteOldConf();
  QFile file(filename.data());
  if (file.open(QFile::ReadOnly) == false)
    {
      //TODO
      std::cerr << "open failed: " << filename << std::endl;
      return ;
    }
  QDomDocument dom;
  dom.setContent(&file);
  file.close();
  QDomElement node = dom.documentElement();

  node = node.firstChildElement();
  while (!node.isNull())
    {
      if (node.tagName().toStdString() == std::string("VirtualHost"))
	this->parseVirtualHost(node);
      else if (node.tagName().toStdString() == std::string("zia"))
	this->parseMainConf(node);
      else if (node.tagName().toStdString() == std::string("Module"))
	this->parseModule(node);
      node = node.nextSiblingElement();
    }
}

const std::string	Conf::get(const std::string& option, const std::string& default_value, const std::string& port, const std::string& h)
{
  std::vector<std::map<std::string, std::string> >::iterator it;
  // if the host is something like localhost:8000, remove the port to get only localhost
  size_t sep = h.find_first_of(":");
  std::string host;
  if (sep == std::string::npos)
    host = h;
  else
    host = h.substr(0, sep);
  // search for an host with the correct port AND correct host
  for (it = this->v_hosts.begin(); it != this->v_hosts.end(); ++it)
    {
      if ((*it)["port"] == port)
	{
	  if ((*it)["host"] == host)
	    {
	      if ((*it)[option] != "")
		return (*it)[option];
	      break ;
	    }
	}
    }
  // then search for an host with the correct port and NO host specified
  for (it = this->v_hosts.begin(); it != this->v_hosts.end(); ++it)
    {
      if ((*it)["port"] == port)
	{
	  if ((*it)["host"] == "")
	    {
	      if ((*it)[option] != "")
		return (*it)[option];
	      // return default_value;
	      break ;
	    }
	}
    }
  // then search for an host with the correct port and any host
  for (it = this->v_hosts.begin(); it != this->v_hosts.end(); ++it)
    {
      if ((*it)["port"] == port)
	{
	  if ((*it)[option] != "")
	    return (*it)[option];
	}
    }
  // then return the main configuration value
  return this->get(option, default_value);
}

const std::string	Conf::get(const std::string& option, const std::string& default_value)
{
  if (this->default_conf[option] == "")
    return default_value;
  return this->default_conf[option];
}

ModConf*	Conf::getModConf(const std::string& name)
{
  std::map<std::string, ModConf>::iterator it;
  it = this->mods.find(name);
  if (it == this->mods.end())
    {
      return NULL;
    }
  return &(it->second);
}

void		Conf::clearConf(void)
{
  this->default_conf.clear();
  this->v_hosts.clear();
  this->mods.clear();
}

Conf* Conf::pInstance = NULL;
