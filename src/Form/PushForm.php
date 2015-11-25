<?php

namespace Drupal\deploy\Form;

use Drupal\Core\Form\FormBase;
use Drupal\Core\Form\FormStateInterface;
use Drupal\Core\Render\RendererInterface;
use Drupal\deploy\Plugin\EndpointManager;
use Drupal\multiversion\Workspace\WorkspaceManagerInterface;
use Symfony\Component\DependencyInjection\ContainerInterface;
use Drupal\deploy\DeployInterface;
use Drupal\Core\Ajax\AjaxResponse;
use Drupal\Core\Ajax\CloseModalDialogCommand;
use Drupal\Core\Ajax\HtmlCommand;
use Drupal\Core\Ajax\CssCommand;

/**
 * Class PushForm
 * @package Drupal\deploy\Form
 */
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
   * @var EndpointManager
   */
  protected $manager;


  /**
   * @var
   */
  protected $user;

  /**
   * @param \Drupal\multiversion\Workspace\WorkspaceManagerInterface $workspace_manager
   */
  function __construct(WorkspaceManagerInterface $workspace_manager, DeployInterface $deploy, EndpointManager $manager, RendererInterface $renderer, $user) {
    $this->workspaceManager = $workspace_manager;
    $this->deploy = $deploy;
    $this->manager = $manager;
    $this->renderer = $renderer;
    $this->user = $user;
  }

  /**
   * {@inheritdoc}
   */
  public static function create(ContainerInterface $container) {
    return new static(
    $container->get('workspace.manager'),
    $container->get('deploy.deploy'),
    $container->get('plugin.manager.endpoint.processor'),
    $container->get('renderer'),
    $container->get('current_user')
    );
  }

  /**
   * @return string
   */
  public function getFormId() {
    // Unique ID of the form.
    return 'deploy_form';
  }

  /**
   * @param array $form
   * @param FormStateInterface $form_state
   * @return array
     */
  public function buildForm(array $form, FormStateInterface $form_state) {
    global $base_url;
    
    $workspace_id = $this->workspaceManager->getActiveWorkspace()->id();
    
    $endpoint_definitions = $this->manager->getDefinitions();
    $endpoints = [];
    foreach ($endpoint_definitions as $endpoint_definition) {
      $endpoints[] = $this->manager->createInstance($endpoint_definition['id'])
    }

    $form['message'] = [
      '#markup' => '<div id="deploy-messages"></div>'
    ];

    $form['source'] = [
        '#type' => 'fieldset',
        '#title' => t('Source')
    ];

    $form['target'] = [
        '#type' => 'fieldset',
        '#title' => t('Target')
    ];

    $form['source']['source_domain'] = [
      '#type' => 'textfield',
      '#title' => t('Full url'),
      '#description' => t('E.g. http(s)://{domain}/{path}/{database}'),
      '#placeholder' => $base_url . '/relaxed/' . $workspace_id,
      '#default_value' => $base_url . '/relaxed/' . $workspace_id,
      '#ajax' => [
        'callback' => [$this, 'validateSourceDomainAjax'],
        'event' => 'change',
        'progress' => [
          'type' => 'throbber',
          'message' => t('Verifying url...'),
        ],
      ],
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
        '#ajax' => [
            'callback' => [$this, 'validateTargetDomainAjax'],
            'event' => 'change',
            'progress' => [
                'type' => 'throbber',
                'message' => t('Verifying url...'),
            ],
        ],
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
      '#ajax' => [
        'callback' => [$this, 'submitFormAjax'],
        'event' => 'mousedown',
        'prevent' => 'click',
        'progress' => [
          'type' => 'throbber',
          'message' => 'Pushing deployment',
        ],
      ],
    ];
    $form['cancel'] = [
      '#type' => 'button', 
      '#value' => t('Cancel'),
      '#attributes' => [
        'class' => ['dialog-cancel'],
      ],
    ];
    return $form;
  }

  /**
   * @param array $form
   * @param FormStateInterface $form_state
   * @return AjaxResponse
     */
  public function validateSourceDomainAjax(array &$form, FormStateInterface $form_state) {
    $css = $this->urlCss($form_state->getValue('source_domain'));
    $response = new AjaxResponse();
    $status_messages = ['#type' => 'status_messages'];
    $response->addCommand(new HtmlCommand('#deploy-messages', $this->renderer->renderRoot($status_messages)));
    $response->addCommand(new CssCommand('#edit-source-domain', $css));
    return $response;
  }

  /**
   * @param array $form
   * @param FormStateInterface $form_state
   * @return AjaxResponse
     */
  public function validateTargetDomainAjax(array &$form, FormStateInterface $form_state) {
    $css = $this->urlCss($form_state->getValue('target_domain'));
    $response = new AjaxResponse();
    $status_messages = ['#type' => 'status_messages'];
    $response->addCommand(new HtmlCommand('#deploy-messages', $this->renderer->renderRoot($status_messages)));
    $response->addCommand(new CssCommand('#edit-target-domain', $css));
    return $response;
  }

  /**
   * @param array $form
   * @param FormStateInterface $form_state
     */
  public function validateForm(array &$form, FormStateInterface $form_state) {
    if (!$this->validateUrl($form_state->getValue('source_domain'))) {
      $form_state->setErrorByName('source_domain', $this->t('Invalid source url.'));
    }

    if (!$this->validateUrl($form_state->getValue('target_domain'))) {
      $form_state->setErrorByName('target_domain', $this->t('Invalid target url.'));
    }
  }

  /**
   * @param array $form
   * @param FormStateInterface $form_state
   * @return AjaxResponse
     */
  public function submitFormAjax(array &$form, FormStateInterface $form_state) {
    $result = $this->doDeployment($form_state);
    $response = new AjaxResponse();
    if (!isset($result['error'])) {
      $response->addCommand(new CloseModalDialogCommand());
      drupal_set_message('Successful deployment.');
    }
    else {
      drupal_set_message($result['error'], 'error');
    }
    $status_messages = ['#type' => 'status_messages'];
    $response->addCommand(new HtmlCommand('#deploy-messages', $this->renderer->renderRoot($status_messages)));
    return $response;
  }

  /**
   * @param array $form
   * @param FormStateInterface $form_state
     */
  public function submitForm(array &$form, FormStateInterface $form_state) {
    $result = $this->doDeployment($form_state);
    if (!isset($result['error'])) {
      drupal_set_message('Successful deployment.');
    }
    else {
      drupal_set_message($result['error'], 'error');
    }
  }

  /**
   * @param $domain
   * @return array
     */
  protected function urlCss($domain) {
    $valid_domain = $this->validateUrl($domain);
    if ($valid_domain) {
      return ['border' => '1px solid #00DD00', 'background-color' => '#EEFFEE'];
    }
    else {
      drupal_set_message("Invalid url.", 'error');
      return ['border' => '1px solid #DD0000', 'background-color' => '#FFEEEE'];
    }
  }

  /**
   * @param $domain
   * @return bool
     */
  protected function validateUrl($domain) {
    return (bool) filter_var($domain, FILTER_VALIDATE_URL);
  }

  /**
   * @param FormStateInterface $form_state
   * @return array
     */
  protected function doDeployment(FormStateInterface $form_state) {
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

    return $this->deploy->push($source, $target);
  }
}