<?php

interface DeployProcessor {

  function __construct(DeployAggregator $aggregator, Array $config = array());

  function process($endpoint_name);

  function configForm(&$form_state);

}
