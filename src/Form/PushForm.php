<?php

namespace Drupal\deploy\Form;

use Drupal\Core\Form\FormBase;
use Drupal\Core\Form\FormStateInterface;
use Drupal\Core\Render\RendererInterface;
use Drupal\deploy\Plugin\EndpointManager;
use Symfony\Component\DependencyInjection\ContainerInterface;
use Drupal\deploy\DeployInterface;
use Drupal\Core\Ajax\AjaxResponse;
use Drupal\Core\Ajax\CloseModalDialogCommand;
use Drupal\Core\Ajax\HtmlCommand;
use Drupal\Core\Ajax\CssCommand;
use Drupal\deploy\Entity\Endpoint;

/**
 * Class PushForm
 * @package Drupal\deploy\Form
 */
class PushForm extends FormBase {

  /**
   * @var \Drupal\deploy\Deploy
   */
  protected $deploy;

  /**
   * @var RendererInterface
   */
  protected $renderer;


  /**
   * @param DeployInterface $deploy
   * @param RendererInterface $renderer
   */
  function __construct(DeployInterface $deploy, RendererInterface $renderer) {
    $this->deploy = $deploy;
    $this->renderer = $renderer;
  }

  /**
   * {@inheritdoc}
   */
  public static function create(ContainerInterface $container) {
    return new static(
        $container->get('deploy.deploy'),
        $container->get('renderer')
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

    $endpoint_entities = Endpoint::loadMultiple();
    if (empty($endpoint_entities)) {
      drupal_set_message('Please setup an endpoint before deploying.', 'warning');
      return $this->redirect('entity.endpoint.collection');
    }
    $endpoints = [];
    foreach ($endpoint_entities as $endpoint_entity) {
      $endpoints[$endpoint_entity->id()] = $endpoint_entity->label();
    }

    $form['message'] = [
        '#markup' => '<div id="deploy-messages"></div>'
    ];

    $form['source'] = [
        '#type' => 'select',
        '#title' => t('Source'),
        '#options' => $endpoints
    ];

    $form['target'] = [
        '#type' => 'select',
        '#title' => t('Target'),
        '#options' => $endpoints
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