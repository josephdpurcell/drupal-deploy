<?php

interface DeployProcessor {

  function __construct(DeployProvider $provider);

  function deploy(DeployEndpoint $endpoint);

}
