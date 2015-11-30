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
 *   id = "external",
 *   label = "External Endpoint"
 * )
 */
Class ExternalEndpoint extends EndpointBase {

  /**
   * @inheritDoc
   */
  public function __toString() {
    return '';
  }

  public function buildConfigurationForm(array $form, FormStateInterface $form_state) {
    $form['test'] = ['#markup' => 'Test'];

    return $form;
  }
}