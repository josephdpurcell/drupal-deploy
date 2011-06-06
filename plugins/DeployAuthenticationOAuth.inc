<?php

class DeployAuthenticationOAuth implements DeployAuthentication {

  var $config = array();

  function __construct(Array $config = array()) {
    $this->config = $config;
  }

  function authenticate() {
    return TRUE;
  }

  function configForm(&$form_state) {
    return array();
  }

}
