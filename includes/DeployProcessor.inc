<?php

interface DeployProcessor {

  function __construct(DeployProvider $provider, Array $config = array());

  function process($endpoint_name);

  function configForm(&$form_state);

}
