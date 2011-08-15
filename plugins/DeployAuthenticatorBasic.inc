<?php

class DeployAuthenticatorBasic extends DeployAuthenticatorNone {

  function __construct(DeployService $service, Array $config = array()) {
    $this->service = $service;
    $this->config += array(
      'debug' => FALSE,
      'username' => '',
      'password' => '',
    );
    $this->config = array_merge($this->config, $config);
  }

  function deploy(Traversable $iterator) {
    $this->service->config['headers']['Authorization'] = 'Basic ' . base64_encode($this->config['username'] . ':' . $this->config['password']);
    $this->service->deploy($iterator);
  }

  function configForm(&$form_state) {
    return array(
      'username' => array(
        '#type' => 'textfield',
        '#title' => t('Username'),
        '#description' => t('Enter the username that you want to authenticate with on this endpoint.'),
        '#default_value' => $this->config['username'],
      ),
      'password' => array(
        '#type' => 'password',
        '#title' => t('Password'),
        '#description' => t('Enter the password that you want to authenticate with on this endpoint.'),
        '#default_value' => $this->config['password'],
      ),
    );
  }

}
