<?php

class DeployFetcherViews implements DeployFetcher {

  var $config = array();

  function __construct(array $config) {
    $this->config = $config;
  }

  function getResources() {
    // @todo: Render view, load all entities and return them.
  }

  function configForm(&$form_state) {

  }

}
