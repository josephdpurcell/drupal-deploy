<?php

/**
 * @file
 * contains \Drupal\deploy\Plugin\Endpoint\WorkspaceEndpoint
 */

namespace Drupal\deploy\Plugin\Endpoint;

use Drupal\deploy\Plugin\EndpointBase;
use Drupal\Core\Form\FormStateInterface;

/**
 * @Endpoint(
 *   id = "workspace",
 *   label = "Workspace Endpoint",
 *   deriver = "Drupal\deploy\Plugin\Deriver\WorkspaceDeriver"
 * )
 */
Class WorkspaceEndpoint extends EndpointBase {

  /**
   * @param array $configuration
   * @param string $plugin_id
   * @param mixed $plugin_definition
   */
  public function __construct(array $configuration, $plugin_id, $plugin_definition) {
    parent::__construct($configuration, $plugin_id, $plugin_definition);
    global $base_url;
    $api_root = trim(\Drupal::config('relaxed.settings')->get('api_root'), '/');
    $workspace_id = $this->getPluginDefinition()['dbname'];
    $uri = $base_url . '/' . $api_root . '/' . $workspace_id;
    $this->applyParts(parse_url($uri));
    $this->userInfo = isset($this->configuration['username']) ? $this->configuration['username'] : '';
    if (isset($this->configuration['password'])) {
      $this->userInfo .= ':' . base64_decode($this->configuration['password']);
    }
  }

}