<?php

namespace Drupal\deploy\Form;

use Drupal\Core\Form\FormBase;
use Drupal\Core\Form\FormStateInterface;
use Drupal\multiversion\Workspace\WorkspaceManagerInterface;
use Symfony\Component\DependencyInjection\ContainerInterface;
use Doctrine\CouchDB\CouchDBClient;
use Doctrine\CouchDB\HTTP\HTTPException;
use Doctrine\CouchDB\HTTP\Response;
use Relaxed\Replicator\ReplicationTask;
use Relaxed\Replicator\Replication;
use Drupal\deploy\Deploy;

class PushForm extends FormBase {

  /**
   * @var \Drupal\multiversion\Workspace\WorkspaceManagerInterface
   */
  protected $workspaceManager;

  /**
   * @var \Drupal\deploy\Deploy
   */
  protected $deploy;

  /**
   * @param \Drupal\multiversion\Workspace\WorkspaceManagerInterface $workspace_manager
   */
  function __construct(WorkspaceManagerInterface $workspace_manager, DeployInterface $deploy) {
    $this->workspaceManager = $workspace_manager;
    $this->deploy = $deploy;
  }

  /**
   * {@inheritdoc}
   */
  public static function create(ContainerInterface $container) {
    return new static(
    $container->get('workspace.manager'),
    $container->get('deploy.deploy')
    );
  }

  public function getFormId() {
    // Unique ID of the form.
    return 'deploy_form';
  }

  public function buildForm(array $form, FormStateInterface $form_state) {
    global $base_url;
    
    $workspace_id = $this->workspaceManager->getActiveWorkspace()->id();
    
    // Create a $form API array.
    $form['domain'] = [
      '#type' => 'textfield',
      '#title' => t('Domain'),
      '#default_value' => $base_url,
    ];
    $form['workspace'] = [
      '#type' => 'textfield',
      '#title' => t('Workspace'),
      '#default_value' => $workspace_id,
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
    // Handle submitted form data.
  }
}