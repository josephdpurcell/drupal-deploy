<?php

interface DeployProvider {

  function __construct(Array $config = array());

  function getResources();

  function configForm(&$form_state);

}
