<?php

interface DeployProcessor {

  function __construct(DeployAggregator $aggregator, Array $config = array());

  /**
   * Main process method that should hand over the plan to the endpoint.
   */
  function process($endpoint_name);

  /**
   * Cleanup method where the processors can cleanup after itself.
   */
  function cleanup();

  /**
   * Configuration form.
   */
  function configForm(&$form_state);

}
