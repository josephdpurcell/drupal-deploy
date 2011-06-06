<?php

interface DeployAuthentication {

  function __construct(Array $config = array());

  function authenticate();

  function configForm(&$form_state);

}
