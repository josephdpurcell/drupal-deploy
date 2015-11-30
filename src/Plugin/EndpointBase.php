<?php

/**
 * @file
 * Contains \Drupal\deploy\Plugin\EndpointBase.
 */

namespace Drupal\deploy\Plugin;

use Drupal\Component\Plugin\PluginBase;
use Drupal\Core\Form\FormStateInterface;
use Drupal\Core\Plugin\PluginFormInterface;

/**
 * Base class for Endpoint plugins.
 */
abstract class EndpointBase extends PluginBase implements EndpointInterface, PluginFormInterface {
  /**
   * @inheritDoc
   */
  public function getConfiguration() {
    return $this->configuration;
  }

  /**
   * @inheritDoc
   */
  public function setConfiguration(array $configuration) {
    $this->configuration = $configuration;
  }

  /**
   * @inheritDoc
   */
  public function defaultConfiguration() {
    return [
      'username' => '',
      'password' => '',
    ];
  }

  /**
   * @inheritDoc
   */
  public function calculateDependencies() {
    return [];
  }

  /**
   * @inheritDoc
   */
  public function buildConfigurationForm(array $form, FormStateInterface $form_state) {
    $form['username'] = [
      '#type' => 'textfield',
      '#title' => t('Username'),
      '#required' => TRUE,
      '#default_value' => $this->configuration['username'],
    ];
    $form['password'] = [
      '#type' => 'password',
      '#title' => t('Password'),
      '#required' => TRUE,
    ];

    return $form;
  }

  /**
   * @inheritDoc
   */
  public function validateConfigurationForm(array &$form, FormStateInterface $form_state) {
    if (empty($form_state->getValue('username'))) {
      $form_state->setErrorByName('username', $this->t('Username not set.'));
    }
    if (empty($form_state->getValue('password'))) {
      $form_state->setErrorByName('password', $this->t('Password not set.'));
    }
  }

  /**
   * @inheritDoc
   */
  public function submitConfigurationForm(array &$form, FormStateInterface $form_state) {
    $this->configuration['username'] = $form_state->getValue('username');
    $this->configuration['password'] = base64_encode($form_state->getValue('password'));
  }

}
