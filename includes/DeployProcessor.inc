<?php

interface DeployProcessor {

  function __construct(DeployProvider $provider);

  function process(DeployEndpoint $endpoint);

}
