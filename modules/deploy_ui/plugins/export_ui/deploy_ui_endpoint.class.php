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

    // Endpoint types.
    $endpoint_plugins = deploy_get_endpoint_plugins();
    $options = array();
    foreach ($endpoint_plugins as $key => $endpoint_plugin) {
      $options[$key] = array(
        'name' => $endpoint_plugin['name'],
        'description' => $endpoint_plugin['description'],
      );
    }
    $form['plugin'] = array(
      '#prefix' => '<label>' . t('Endpoint') . '</label>',
      '#type' => 'tableselect',
      '#required' => TRUE,
      '#multiple' => FALSE,
      '#header' => array(
        'name' => t('Name'),
        'description' => t('Description'),
      ),
      '#options' => $options,
      '#default_value' => $item->plugin,
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
    $item->plugin = $form_state['values']['plugin'];
  }

  function edit_form_config(&$form, &$form_state) {
    $item = $form_state['item'];
    // There seems to be differences between update and save in the wizard.
    if (!is_array($item->config)) {
      $item->config = unserialize($item->config);
    }

    // Construct the endpoint object.
    $endpoint = new $item->plugin((array)$item->config);

    $form['config'] = $endpoint->configForm($form_state);
    if (!empty($form['config'])) {
      $form['config']['#tree'] = TRUE;
    }
    else {
      $form['config'] = array(
        '#type' => 'markup',
        '#markup' => '<p>' . t('There are no settings for this endpoint plugin.') . '</p>',
      );
    }
  }

  function edit_form_config_submit(&$form, &$form_state) {
    $item = $form_state['item'];

    if (!empty($form_state['values']['config'])) {
      $item->config = $form_state['values']['config'];
    }
    else {
      $item->config = array();
    }
  }

}
