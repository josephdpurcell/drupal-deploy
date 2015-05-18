<?php

/**
 * @file
 * Contains \Drupal\deploy\Controller\DeployController.
 */

namespace Drupal\deploy\Controller;

class DeployController {

  public function admin() {
    return array(
        '#type' => 'markup',
        '#markup' => t('Insert deploy admin here.'),
    );
  }

}
