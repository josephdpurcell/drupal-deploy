<?php

class deploy_ui_plan extends ctools_export_ui {

  /**
   * Pseudo implementation of hook_menu_alter().
   *
   * @todo
   *   Can we do this in $plugin instead?
   */
  function hook_menu(&$items) {
    parent::hook_menu($items);
    $items['admin/structure/deploy/plans']['type'] = MENU_LOCAL_TASK;
    $items['admin/structure/deploy/plans']['weight'] = -10;
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
    $providers = deploy_get_provider_plugins();
    $options = array();
    foreach ($providers as $key => $provider) {
      $options[$key] = array(
        'name' => $provider['name'],
        'description' => $provider['description'],
      );
    }
    $form['provider_plugin'] = array(
      '#prefix' => '<label>' . t('Provider') . '</label>',
      '#type' => 'tableselect',
      '#required' => TRUE,
      '#multiple' => FALSE,
      '#header' => array(
        'name' => t('Name'),
        'description' => t('Description'),
      ),
      '#options' => $options,
      '#default_value' => $item->provider_plugin,
    );

    // Processors.
    $processors = deploy_get_processor_plugins();
    $options = array();
    foreach ($processors as $key => $processor) {
      $options[$key] = array(
        'name' => $processor['name'],
        'description' => $processor['description'],
      );
    }
    $form['processor_plugin'] = array(
      '#prefix' => '<label>' . t('Processor') . '</label>',
      '#type' => 'tableselect',
      '#required' => TRUE,
      '#multiple' => FALSE,
      '#header' => array(
        'name' => t('Name'),
        'description' => t('Description'),
      ),
      '#options' => $options,
      '#default_value' => $item->processor_plugin,
    );

    // Endpoint types.
    $endpoints = deploy_endpoint_load_all();
    $options = array();
    foreach ($endpoints as $endpoint) {
      $options[$endpoint->name] = array(
        'name' => check_plain($endpoint->title),
        'description' => check_plain($endpoint->description),
      );
    }
    if (!is_array($item->endpoints)) {
      $item->endpoints = unserialize($item->endpoints);
    }
    $form['endpoints'] = array(
      '#prefix' => '<label>' . t('Endpoints') . '</label>',
      '#type' => 'tableselect',
      '#required' => TRUE,
      '#multiple' => TRUE,
      '#header' => array(
        'name' => t('Name'),
        'description' => t('Description'),
      ),
      '#options' => $options,
      '#default_value' => (array)$item->endpoints,
    );
  }

  /**
   * Submit callback for basic config.
   */
  function edit_form_submit(&$form, &$form_state) {
    $item = $form_state['item'];

    $item->name = $form_state['values']['name'];
    $item->title = $form_state['values']['title'];
    $item->description = $form_state['values']['description'];
    $item->provider_plugin = $form_state['values']['provider_plugin'];
    $item->processor_plugin = $form_state['values']['processor_plugin'];
    if (!empty($form_state['values']['endpoints'])) {
      $item->endpoints = $form_state['values']['endpoints'];
    }
    else {
      $item->endpoints = array();
    }
  }

  function edit_form_provider(&$form, &$form_state) {
    $item = $form_state['item'];
    if (!is_array($item->provider_config)) {
      $item->provider_config = unserialize($item->provider_config);
    }

    // Create the provider object.
    $provider = new $item->provider_plugin((array)$item->provider_config);

    $form['provider_config'] = $provider->configForm($form_state);
    if (!empty($form['provider_config'])) {
      $form['provider_config']['#tree'] = TRUE;
    }
    else {
      $form['provider_config'] = array(
        '#type' => 'markup',
        '#markup' => '<p>' . t('There are no settings for this provider plugin.') . '</p>'
      );
    }
  }

  function edit_form_provider_submit(&$form, &$form_state) {
    $item = $form_state['item'];
    if (!empty($form_state['values']['provider_config'])) {
      $item->provider_config = $form_state['values']['provider_config'];
    }
    else {
      $item->provider_config = array();
    }
  }

  function edit_form_processor(&$form, &$form_state) {
    $item = $form_state['item'];
    if (!is_array($item->processor_config)) {
      $item->processor_config = unserialize($item->processor_config);
    }

    // Create the provider object which is a dependency of the processor object.
    $provider = new $item->provider_plugin((array)$item->provider_config);
    // Create the processor object.
    $processor = new $item->processor_plugin($provider, (array)$item->processor_config);

    $form['processor_config'] = $processor->configForm($form_state);
    if (!empty($form['config']['processor_config'])) {
      $form['processor_config']['#tree'] = TRUE;
    }
    else {
      $form['config']['processor_config'] = array(
        '#type' => 'markup',
        '#markup' => '<p>' . t('There are no settings for this processor plugin.') . '</p>'
      );
    }
  }

  function edit_form_processor_submit(&$form, &$form_state) {
    $item = $form_state['item'];
    if (!empty($form_state['values']['processor_config'])) {
      $item->processor_config = $form_state['values']['processor_config'];
    }
    else {
      $item->processor_config = array();
    }
  }

}
