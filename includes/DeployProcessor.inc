<?php

interface DeployProcessor {

  function __construct(DeployProvider $provider, Array $config = array());

  function process(DeployEndpoint $endpoint);

  function configForm(&$form_state);

}
