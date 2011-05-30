<?php

class deploy_ui_setup extends ctools_export_ui {

  /**
   * Pseudo implementation of hook_menu_alter().
   */
  function hook_menu(&$items) {
    parent::hook_menu($items);
    $items['admin/structure/deploy/setups']['type'] = MENU_LOCAL_TASK;
  }

  /**
   * Form callback for basic config.
   */
  function edit_form(&$form, &$form_state) {
    $item = $form_state['item'];

    // Basics.
    $form['name'] = array(
      '#type' => 'textfield',
      '#title' => t('Machine-readable name'),
      '#default_value' => $item->name,
      '#required' => TRUE,
    );
    $form['title'] = array(
      '#type' => 'textfield',
      '#title' => t('Title'),
      '#default_value' => $item->title,
      '#required' => TRUE,
    );
    $form['description'] = array(
      '#type' => 'textarea',
      '#title' => t('Description'),
      '#default_value' => $item->description,
    );

    // Fetchers.
    $fetchers = deploy_get_fetchers();
    $options = array();
    foreach ($fetchers as $key => $fetcher) {
      $options[$key] = array(
        'name' => $fetcher['name'],
        'description' => $fetcher['description'],
      );
    }
    $form['fetcher'] = array(
      '#prefix' => '<label>' . t('Fetcher') . '</label>',
      '#type' => 'tableselect',
      '#required' => TRUE,
      '#multiple' => FALSE,
      '#header' => array(
        'name' => t('Name'),
        'description' => t('Description'),
      ),
      '#options' => $options,
      '#default_value' => $item->fetcher,
    );

    // Workers.
    $workers = deploy_get_workers();
    $options = array();
    foreach ($workers as $key => $worker) {
      $options[$key] = array(
        'name' => $worker['name'],
        'description' => $worker['description'],
      );
    }
    $form['worker'] = array(
      '#prefix' => '<label>' . t('Worker') . '</label>',
      '#type' => 'tableselect',
      '#required' => TRUE,
      '#multiple' => FALSE,
      '#header' => array(
        'name' => t('Name'),
        'description' => t('Description'),
      ),
      '#options' => $options,
      '#default_value' => $item->worker,
    );

    // @todo: Add tableselect for endpoints.
  }

  /**
   * Submit callback for basic config.
   */
  function edit_form_submit(&$form, &$form_state) {
    $form_state['item']->name = $form_state['values']['name'];
    $form_state['item']->title = $form_state['values']['title'];
    $form_state['item']->description = $form_state['values']['description'];
    $form_state['item']->fetcher = $form_state['values']['fetcher'];
    $form_state['item']->worker = $form_state['values']['worker'];
  }

  function edit_form_fetcher(&$form, &$form_state) {

  }

  function edit_form_fetcher_submit(&$form, &$form_state) {

  }

  function edit_form_worker(&$form, &$form_state) {

  }

  function edit_form_worker_submit(&$form, &$form_state) {

  }

  function edit_form_endpoint(&$form, &$form_state) {

  }

  function edit_form_endpoint_submit(&$form, &$form_state) {

  }

}
