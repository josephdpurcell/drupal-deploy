<?php

namespace Drupal\deploy;

interface DeployInterface {

  public function push($target_domain, $target_username, $target_password, $target_workspace);

}