<?php

class deploy_ui_plan extends ctools_export_ui {

  /**
   * Pseudo implementation of hook_menu_alter().
   */
  function hook_menu(&$items) {
    parent::hook_menu($items);
    $items['admin/structure/deploy/plans']['type'] = MENU_LOCAL_TASK;
  }

  /**
   * Form callback for basic config.
   */
  function edit_form(&$form, &$form_state) {
    $item = $form_state['item'];

    // Basics.
    $form['title'] = array(
      '#type' => 'textfield',
      '#title' => t('Title'),
      '#default_value' => $item->title,
      '#required' => TRUE,
    );
    $form['name'] = array(
      '#type' => 'machine_name',
      '#title' => t('Machine-readable name'),
      '#default_value' => $item->name,
      '#required' => TRUE,
      '#machine_name' => array(
        'exists' => 'deploy_plan_load',
        'source' => array('title'),
      ),
    );
    $form['description'] = array(
      '#type' => 'textarea',
      '#title' => t('Description'),
      '#default_value' => $item->description,
    );

    // Providers.
    $providers = deploy_get_providers();
    $options = array();
    foreach ($providers as $key => $provider) {
      $options[$key] = array(
        'name' => $provider['name'],
        'description' => $provider['description'],
      );
    }
    $form['provider'] = array(
      '#prefix' => '<label>' . t('Provider') . '</label>',
      '#type' => 'tableselect',
      '#required' => TRUE,
      '#multiple' => FALSE,
      '#header' => array(
        'name' => t('Name'),
        'description' => t('Description'),
      ),
      '#options' => $options,
      '#default_value' => $item->provider,
    );

    // Processors.
    $processors = deploy_get_processors();
    $options = array();
    foreach ($processors as $key => $processor) {
      $options[$key] = array(
        'name' => $processor['name'],
        'description' => $processor['description'],
      );
    }
    $form['processor'] = array(
      '#prefix' => '<label>' . t('Processor') . '</label>',
      '#type' => 'tableselect',
      '#required' => TRUE,
      '#multiple' => FALSE,
      '#header' => array(
        'name' => t('Name'),
        'description' => t('Description'),
      ),
      '#options' => $options,
      '#default_value' => $item->processor,
    );

    // @todo: Add tableselect for endpoints.
  }

  /**
   * Submit callback for basic config.
   */
  function edit_form_submit(&$form, &$form_state) {
    $item = $form_state['item'];

    $item->name = $form_state['values']['name'];
    $item->title = $form_state['values']['title'];
    $item->description = $form_state['values']['description'];
    $item->provider = $form_state['values']['provider'];
    $item->processor = $form_state['values']['processor'];
  }

  function edit_form_provider(&$form, &$form_state) {
    $item = $form_state['item'];
    // There seems to be differences between update and save in the wizard.
    if (!is_array($item->config)) {
      $item->config = unserialize($item->config);
    }

    $provider_class = $item->provider;

    // Construct the provider object.
    $provider = new $provider_class((array)$item->config['provider']);

    $form['config'] = array('#tree' => TRUE);
    $form['config']['provider'] = $provider->configForm($form_state);

    if (empty($form['config']['provider'])) {
      $form['config']['provider'] = array(
        '#type' => 'markup',
        '#markup' => '<p>' . t('There are no settings for this provider plugin.') . '</p>'
      );
    }
  }

  function edit_form_provider_submit(&$form, &$form_state) {
    $item = $form_state['item'];
    $item->config['provider'] = $form_state['values']['config']['provider'];
  }

  function edit_form_processor(&$form, &$form_state) {
    $item = $form_state['item'];
    // There seems to be differences between update and save in the wizard.
    if (!is_array($item->config)) {
      $item->config = unserialize($item->config);
    }

    $provider_class = $item->provider;
    $processor_class = $item->processor;

    // Construct the provider object which is a dependency of the processor.
    $provider = new $provider_class((array)$item->config['provider']);
    // Construct the processor object.
    $processor = new $processor_class($provider, (array)$item->config['processor']);

    $form['config'] = array('#tree' => TRUE);
    $form['config']['processor'] = $processor->configForm($form_state);

    if (empty($form['config']['processor'])) {
      $form['config']['processor'] = array(
        '#type' => 'markup',
        '#markup' => '<p>' . t('There are no settings for this processor plugin.') . '</p>'
      );
    }
  }

  function edit_form_processor_submit(&$form, &$form_state) {
    $item = $form_state['item'];
    $item->config['processor'] = $form_state['values']['config']['processor'];
  }

  function edit_form_endpoint(&$form, &$form_state) {

  }

  function edit_form_endpoint_submit(&$form, &$form_state) {

  }

}
