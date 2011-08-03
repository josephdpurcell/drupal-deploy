<?php

interface DeployProcessor {

  function __construct(DeployAggregator $aggregator, Array $config = array());

  function process(DeployPlan $plan, $endpoint_name);

  function configForm(&$form_state);

}
