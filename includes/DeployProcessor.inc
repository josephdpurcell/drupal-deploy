<?php

/**
 * The interface for deployment processors.
 */
interface DeployProcessor {

  /**
   * Constructor for a deploy processor.
   *
   * @param DeployAggregator $aggregator
   *   The aggregator object that will be processed.
   *
   * @param array $config
   *   An associative array of configuration settings for the processor.
   */
  public function __construct(DeployAggregator $aggregator, Array $config = array());

  /**
   * Main processing method that should hand over the aggregator to the endpoint
   * for deployment.
   *
   * @param string $endpoint_name
   *   The name of the endpoint being deployed to.
   */
  public function deploy($endpoint_name);

  /**
   * Cleanup method where the processors can cleanup after itself.
   */
  public function cleanup();

  /**
   * Configuration form.
   *
   * @param array $form_state
   *   The complete form state.
   */
  public function configForm(&$form_state);

}
