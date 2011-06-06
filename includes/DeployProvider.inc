<?php

interface DeployProvider {

  function __construct(Array $config = array());

  function getEntities();

  function configForm(&$form_state);

}
