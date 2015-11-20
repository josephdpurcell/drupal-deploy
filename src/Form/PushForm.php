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

    $form['source']['source_domain'] = [
      '#type' => 'textfield',
      '#title' => t('Full url'),
      '#description' => t('E.g. http(s)://{domain}/{path}/{database}'),
      '#placeholder' => $base_url . '/relaxed/' . $workspace_id,
      '#default_value' => $base_url . '/relaxed/' . $workspace_id,
    ];
    $form['source']['source_username'] = [
        '#type' => 'textfield',
        '#title' => t('username'),
        '#default_value' => $this->user->getAccountName(),
    ];
    $form['source']['source_password'] = [
        '#type' => 'password',
        '#title' => t('Password'),
    ];

    $form['target']['target_domain'] = [
        '#type' => 'textfield',
        '#title' => t('Full url'),
        '#description' => t('E.g. http(s)://{domain}/{path}/{database}'),
        '#placeholder' => $base_url . '/relaxed/' . $workspace_id,
        '#default_value' => $base_url . '/relaxed/' . $workspace_id,
    ];
    $form['target']['target_username'] = [
        '#type' => 'textfield',
        '#title' => t('username'),
        '#default_value' => $this->user->getAccountName(),
    ];
    $form['target']['target_password'] = [
        '#type' => 'password',
        '#title' => t('Password'),
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

  public function submitForm(array &$form, FormStateInterface $form_state) {
    $source = $this->deploy->createSource(
      $form_state->getValue('source_domain'),
      $form_state->getValue('source_username'),
      $form_state->getValue('source_password')
    );
    $target = $this->deploy->createTarget(
      $form_state->getValue('target_domain'),
      $form_state->getValue('target_username'),
      $form_state->getValue('target_password')
    );
    $result = $this->deploy->push($source, $target);
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