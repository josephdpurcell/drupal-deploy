<?php

interface DeployAuthentication {

  function __construct(DeployService $service, Array $config = array());

  function deploy(Array $entity_wrappers);

  function configForm(&$form_state);

}
