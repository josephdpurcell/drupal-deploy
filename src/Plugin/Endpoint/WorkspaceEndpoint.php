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
     * @inheritDoc
     */
    public function __toString() {
        global $base_url;
        $plugin_definition = $this->getPluginDefinition();
        return $base_url . '/relaxed/' . urlencode($plugin_definition['dbname']);
    }

    public function buildConfigurationForm(array $form, FormStateInterface $form_state) {
        $form += parent::buildConfigurationForm($form, $form_state);
        $form['test'] = ['#markup' => 'Test'];

        return $form;
    }
}