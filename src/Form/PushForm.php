<?php

namespace Drupal\deploy\Form;

use Drupal\Core\Form\FormBase;
use Drupal\Core\Form\FormStateInterface;

class PushForm extends FormBase {
  public function getFormId() {
    // Unique ID of the form.
    return 'deploy_form';
  }

  public function buildForm(array $form, FormStateInterface $form_state) {
    // Create a $form API array.
    $form['domain'] = [
      '#type' => 'textfield',
      '#title' => t('Domain'),
    ];
    $form['workspace'] = [
      '#type' => 'textfield',
      '#title' => t('Workspace'),
    ];
    $form['tag'] = [
      '#type' => 'textfield',
      '#title' => t('Tag'),
    ];
    $form['push'] = [
      '#type' => 'submit', 
      '#value' => t('Push'), 
      '#button_type' => 'primary',
      '#ajax' => array(
            'callback' => 'Drupal\deploy\Form\PushForm::submitForm',
            'event' => 'click',
            'progress' => array(
              'type' => 'throbber',
              'message' => 'Pushing deployment',
            ),
        
          ),
    ];
    $form['cancel'] = [
      '#type' => 'button', 
      '#value' => t('Cancel'),
    ];
    return $form;
  }

  public function validateForm(array &$form, FormStateInterface $form_state) {
    // Validate submitted form data.
  }

  public function submitForm(array &$form, FormStateInterface $form_state) {
    // Handle submitted form data.
  }
}