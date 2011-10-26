<?php

/**
 * Class for deploying to a REST server using JSON
 */
class DeployServiceRestJSON extends DeployServiceRest {

  public function deploy(Traversable $iterator) {
    foreach ($iterator as $current) {
      $data = drupal_json_encode($current);
      $this->config['headers']['Content-Type'] = 'application/json';
      $response = $this->httpPut($current['entity']->uuid, $data, 'json');
      // TODO: Change eid to entity_id.
      $args = array('eid' => $current['entity_id'], 'response' => $response, 'plan' => $this->plan->name);
      module_invoke_all('deploy_item_deployed', $this, $args);
    }
  }
}
