<?php

interface DeployService {

  function __construct(Array $config = array());

  function deploy(DeployEntityIteratorIterator $iterator);

  function configForm(&$form_state);

}
