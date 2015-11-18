<?php

namespace Drupal\deploy\Form;

use Drupal\Core\Form\FormBase;
use Drupal\Core\Form\FormStateInterface;
use Drupal\multiversion\Workspace\WorkspaceManagerInterface;
use Symfony\Component\DependencyInjection\ContainerInterface;
use Drupal\deploy\DeployInterface;
use Drupal\Core\Ajax\AjaxResponse;
use Drupal\Core\Ajax\CloseModalDialogCommand;

class PushForm extends FormBase {

  /**
   * @var \Drupal\multiversion\Workspace\WorkspaceManagerInterface
   */
  protected $workspaceManager;

  /**
   * @var \Drupal\deploy\Deploy
   */
  protected $deploy;

  protected $user;

  /**
   * @param \Drupal\multiversion\Workspace\WorkspaceManagerInterface $workspace_manager
   */
  function __construct(WorkspaceManagerInterface $workspace_manager, DeployInterface $deploy, $user) {
    $this->workspaceManager = $workspace_manager;
    $this->deploy = $deploy;
    $this->user = $user;
  }

  /**
   * {@inheritdoc}
   */
  public static function create(ContainerInterface $container) {
    return new static(
    $container->get('workspace.manager'),
    $container->get('deploy.deploy'),
    $container->get('current_user')
    );
  }

  public function getFormId() {
    // Unique ID of the form.
    return 'deploy_form';
  }

  public function buildForm(array $form, FormStateInterface $form_state) {
    global $base_url;
    
    $workspace_id = $this->workspaceManager->getActiveWorkspace()->id();

    $form['source'] = array(
        '#type' => 'fieldset',
        '#title' => t('Source')
    );

    $form['target'] = array(
        '#type' => 'fieldset',
        '#title' => t('Target')
    );

    $form['source']['domain'] = [
      '#type' => 'textfield',
      '#title' => t('Domain'),
      '#default_value' => $base_url,
    ];
    $form['source']['username'] = [
        '#type' => 'textfield',
        '#title' => t('username'),
        '#default_value' => $this->user->getAccountName(),
    ];
    $form['source']['password'] = [
        '#type' => 'password',
        '#title' => t('Password'),
    ];
    $form['source']['workspace'] = [
      '#type' => 'textfield',
      '#title' => t('Workspace'),
      '#default_value' => $workspace_id,
    ];

    $form['target']['domain'] = [
        '#type' => 'textfield',
        '#title' => t('Domain'),
        '#default_value' => $base_url,
    ];
    $form['target']['username'] = [
        '#type' => 'textfield',
        '#title' => t('username'),
        '#default_value' => $this->user->getAccountName(),
    ];
    $form['target']['password'] = [
        '#type' => 'password',
        '#title' => t('Password'),
    ];
    $form['target']['workspace'] = [
        '#type' => 'textfield',
        '#title' => t('Workspace'),
        '#default_value' => $workspace_id,
    ];

    $form['push'] = [
      '#type' => 'submit', 
      '#value' => t('Push'), 
      '#button_type' => 'primary',
      '#ajax' => array(
            'callback' => '::submitForm',
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
      '#attributes' => array(
        'class' => array('dialog-cancel'),
      ),
    ];
    return $form;
  }

  public function validateForm(array &$form, FormStateInterface $form_state) {
    // Validate submitted form data.
  }

  public function submitForm(array &$form, FormStateInterface $form_state)
  {
    $result = $this->deploy->push(
        $form_state->getValue('domain'),
        $form_state->getValue('username'),
        $form_state->getValue('password'),
        $form_state->getValue('workspace')
    );
    $this->logger('Deploy')->info(print_r($result, true));
    $response = false;
    if ($result) {
      $command = new CloseModalDialogCommand();
      $response = new AjaxResponse();
      $response->addCommand($command);
    }
    return $response;
  }
}