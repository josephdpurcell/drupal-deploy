<?php

class deploy_ui_endpoint extends ctools_export_ui {

  /**
   * Pseudo implementation of hook_menu_alter().
   *
   * @todo
   *   Can we do this in $plugin instead?
   */
  function hook_menu(&$items) {
    parent::hook_menu($items);
    $items['admin/structure/deploy/endpoints']['type'] = MENU_LOCAL_TASK;
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
        'exists' => 'deploy_endpoint_load',
        'source' => array('title'),
      ),
    );
    $form['description'] = array(
      '#type' => 'textarea',
      '#title' => t('Description'),
      '#default_value' => $item->description,
    );

    // Authentications.
    $services = deploy_get_authentication_plugins();
    $options = array();
    foreach ($authentications as $key => $authentication) {
      $options[$key] = array(
        'name' => $authentication['name'],
        'description' => $authentication['description'],
      );
    }
    $form['authentication_plugin'] = array(
      '#prefix' => '<label>' . t('Authentication') . '</label>',
      '#type' => 'tableselect',
      '#required' => TRUE,
      '#multiple' => FALSE,
      '#header' => array(
        'name' => t('Name'),
        'description' => t('Description'),
      ),
      '#options' => $options,
      '#default_value' => $item->authentication_plugin,
    );

    // Services.
    $services = deploy_get_service_plugins();
    $options = array();
    foreach ($services as $key => $service) {
      $options[$key] = array(
        'name' => $service['name'],
        'description' => $service['description'],
      );
    }
    $form['service_plugin'] = array(
      '#prefix' => '<label>' . t('Service') . '</label>',
      '#type' => 'tableselect',
      '#required' => TRUE,
      '#multiple' => FALSE,
      '#header' => array(
        'name' => t('Name'),
        'description' => t('Description'),
      ),
      '#options' => $options,
      '#default_value' => $item->service_plugin,
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
    $item->authentication_plugin = $form_state['values']['authentication_plugin'];
    $item->service_plugin = $form_state['values']['service_plugin'];
  }

  function edit_form_authentication(&$form, &$form_state) {
    $item = $form_state['item'];
    if (!is_array($item->authentication_config)) {
      $item->authentication_config = unserialize($item->authentication_config);
    }

    // Create the authentication object.
    $authentication = new $item->authentication_plugin((array)$item->authentication_config);

    $form['authentication_config'] = $authentication->configForm($form_state);
    if (!empty($form['authentication_config'])) {
      $form['authentication_config']['#tree'] = TRUE;
    }
    else {
      $form['authentication_config'] = array(
        '#type' => 'markup',
        '#markup' => '<p>' . t('There are no settings for this authentication plugin.') . '</p>',
      );
    }
  }

  function edit_form_authentication_submit(&$form, &$form_state) {
    $item = $form_state['item'];
    if (!empty($form_state['values']['authentication_config'])) {
      $item->authentication_config = $form_state['values']['authentication_config'];
    }
    else {
      $item->authentication_config = array();
    }
  }

  function edit_form_service(&$form, &$form_state) {
    $item = $form_state['item'];
    if (!is_array($item->service_config)) {
      $item->service_config = unserialize($item->service_config);
    }

    // Create the service object.
    $service = new $item->service_plugin((array)$item->service_config);

    $form['service_config'] = $service->configForm($form_state);
    if (!empty($form['service_config'])) {
      $form['service_config']['#tree'] = TRUE;
    }
    else {
      $form['service_config'] = array(
        '#type' => 'markup',
        '#markup' => '<p>' . t('There are no settings for this service plugin.') . '</p>',
      );
    }
  }

  function edit_form_service_submit(&$form, &$form_state) {
    $item = $form_state['item'];
    if (!empty($form_state['values']['service_config'])) {
      $item->service_config = $form_state['values']['service_config'];
    }
    else {
      $item->service_config = array();
    }
  }

}
