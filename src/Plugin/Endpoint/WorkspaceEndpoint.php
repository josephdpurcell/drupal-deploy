<?php

/**
 * @file
 * contains \Drupal\deploy\Plugin\Endpoint\WorkspaceEndpoint
 */

namespace Drupal\deploy\Plugin\Endpoint;

use Drupal\deploy\Plugin\EndpointBase;

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
}