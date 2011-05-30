<?php

interface DeployFetcher {

  function __construct(Array $config);

  function getResources();

  function configForm(&$form_state);

}
