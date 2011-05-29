<?php

interface DeployEndpoint {

  function __construct(Array $config);

  function render(DeployResource $resource);

}
