<?php

interface DeployService {

  function __construct(Array $config = array());

  function deploy(Array $entities);

  function configForm(&$form_state);

}