<?php

interface DeployProvider {

  function __construct(Array $config = array());

  function getEntityIds();

  function configForm(&$form_state);

}
