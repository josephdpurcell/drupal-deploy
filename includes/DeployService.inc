<?php

interface DeployService {

  function __construct(Array $config = array());

  function deploy(Array $entity_wrappers);

  function setHeader($key, $value);

  function configForm(&$form_state);

}
