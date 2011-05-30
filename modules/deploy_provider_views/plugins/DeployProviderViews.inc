<?php

class DeployProviderViews implements DeployProvider {

  var $config = array();

  function __construct(Array $config) {
    $this->config = $config;
  }

  function getResources() {
    // @todo: Render view, load all entities and return them.
  }

  function configForm(&$form_state) {

  }

}
