<?php

/**
 * @file
 * Contains \Drupal\deploy\Form\EndpointForm.
 */

namespace Drupal\deploy\Form;

use Drupal\Core\Entity\EntityInterface;
use Drupal\Core\Entity\EntityForm;
use Drupal\Core\Form\FormStateInterface;

/**
 * Class EndpointForm.
 *
 * @package Drupal\deploy\Form
 */
class EndpointForm extends EntityForm {
  /**
   * {@inheritdoc}
   */
  public function form(array $form, FormStateInterface $form_state) {
    $form = parent::form($form, $form_state);

    $endpoint = $this->entity;
    $form['label'] = array(
      '#type' => 'textfield',
      '#title' => $this->t('Label'),
      '#maxlength' => 255,
      '#default_value' => $endpoint->label(),
      '#description' => $this->t("Label for the Endpoint."),
      '#required' => TRUE,
    );

    $form['id'] = array(
      '#type' => 'machine_name',
      '#default_value' => $endpoint->id(),
      '#machine_name' => array(
        'exists' => '\Drupal\deploy\Entity\Endpoint::load',
      ),
      '#disabled' => !$endpoint->isNew(),
    );

    /* You will need additional form elements for your custom properties. */

    return $form;
  }

  /**
   * {@inheritdoc}
   */
  public function save(array $form, FormStateInterface $form_state) {
    $endpoint = $this->entity;
    $status = $endpoint->save();

    switch ($status) {
      case SAVED_NEW:
        drupal_set_message($this->t('Created the %label Endpoint.', [
          '%label' => $endpoint->label(),
        ]));
        break;

      default:
        drupal_set_message($this->t('Saved the %label Endpoint.', [
          '%label' => $endpoint->label(),
        ]));
    }
    $form_state->setRedirectUrl($endpoint->urlInfo('collection'));
  }

}
