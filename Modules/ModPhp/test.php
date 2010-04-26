#!/usr/bin/php
<?php

$someDirs = array('./', './../');
foreach($someDirs as $Dir)
{
  if(($openIt = opendir($Dir)) != FALSE)
    while($file = readdir($openIt))
      echo realpath("$Dir$file")."\n";
}

echo "_POST"."\n";
var_dump($_POST);
echo "_GET"."\n";
var_dump($_GET);
?>