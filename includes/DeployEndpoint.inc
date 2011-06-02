<?php

interface DeployEndpoint {

  function __construct(Array $config = array());

  function deploy(Array $resources);

  function configForm(&$form_state);

}
