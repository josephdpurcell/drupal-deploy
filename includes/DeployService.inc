<?php

interface DeployService {

  function __construct(Array $config = array());

  function deploy(Array $entity_wrappers);

  function configForm(&$form_state);

}
