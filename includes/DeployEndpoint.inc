<?php

interface DeployEndpoint {

  function __construct(Array $config);

  function deploy(DeployResource $resource);

}