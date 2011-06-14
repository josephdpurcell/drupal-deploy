<?php

interface DeployAuthenticator {

  function __construct(DeployService $service, Array $config = array());

  function deploy(Traversable $iterator);

  function configForm(&$form_state);

}
