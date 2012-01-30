<?php

/**
 * Wrapper class around the SPL RecursiveIteratorIterator.
 *
 * @todo
 *   We need to implement some static caching of the inner iterator here. One
 *   deployment involves a lot of iteration around the DeployIterator which is
 *   heavy.
 */
class DeployIteratorIterator extends RecursiveIteratorIterator {}

/**
 * Iterator class which does the heavy lifting for detecting dependencies.
 *
 * This iterator is reponsible for taking in an array of entity types and ids,
 * figuring out all their dependencies, and returning an iterable object of all
 * of them in a deployable order (dependencies first). And since dependencies
 * in theory are nested and recursive, we are using a recursive iterator here.
 *
 * @todo
 *   We need to throw an exception when we detect a circular dependency.
 *
 * @todo
 *   Look into Entity Property API to see if we can do anything cool/effective
 *   with the chaining feature there.
 */
class DeployIterator implements RecursiveIterator {

  /**
   * The entities to be iterated over.
   *
   * @var array
   */
  public $entities = array();

  /**
   * Keeps track of the entity type currently being iterated over.
   *
   * @var string
   */
  public $entityType = NULL;

  /**
   * An array of dependencies to the entity being parsed.
   *
   * @var array
   */
  public $dependencies = array();

  /**
   * An array of belongings to the entity being parsed.
   *
   * @var array
   */
  public $belongings = array();

  /**
   * An array with information on who added a particular entity to the tree,
   * i.e. an entity's origin.
   *
   * @var array
   */
  public $origins = array();

  /**
   * Keeps track of entities that have already been checked for dependencies.
   *
   * @var array
   */
  public $checked = array();

  /**
   * Keeps track of entities that have already been traversed (output).
   *
   * @var array
   */
  public $traversed = array();

  /**
   * Constructor.
   *
   * The $entities array should be structured as below, where all string keys
   * are entity types and the numeric keys are entity ids.
   * @code
   *   $entities = array(
   *     'node' => array(
   *       10 => TRUE,
   *       12 => TRUE,
   *     ),
   *     'taxonomy_term' => array(
   *       16 => TRUE,
   *     ),
   *   );
   * @endcode
   *
   * @param array $entities
   *   A structured array of entity ids and their entity types.
   *
   * @param array $parent
   *   The parent array of the current entity.
   */
  public function __construct($entities, &$parent = NULL) {
    $this->entities = $entities;

    if (empty($parent)) {
      foreach ($this->entities as $entity_type => $entity_ids) {
        foreach ($entity_ids as $entity_id => $somethings) {
          $this->origins[$entity_type][$entity_id] = FALSE;
        }
      }
    }
    else {
      $this->origins =& $parent->origins;
      $this->checked =& $parent->checked;
      $this->traversed =& $parent->traversed;
    }
  }

  /**
   * Returns TRUE if an iterator can be created for the current item in the
   * entities array.
   *
   * @return boolean
   */
  public function hasChildren() {
    $current = current($this->entities);
    if (is_array($current)) {
      return TRUE;
    }

    // Don't check for dependencies twice.
    if (!isset($this->checked[$this->entityType][$this->entityId]) && !empty($this->entityId)) {
      // Since the current entity is a UUID entity (with all properties and
      // field references as UUIDs) we need to re-load it as a standard entity
      // to make life easier for implementors of the following hooks. This
      // should have minor performance implications since the entity should
      // be cached at this point.
      $entity = reset(entity_load($this->entityType, array($this->entityId)));

      $this->dependencies = module_invoke_all('deploy_entity_dependencies', $entity, $this->entityType);
      $this->belongings = module_invoke_all('deploy_entity_belongings', $entity, $this->entityType);

      // Don't add dependencies that already were checked.
      // TODO: Do the same for belongings?
      foreach ($this->dependencies as $entity_type => $dependencies) {
        foreach ($dependencies as $entity_id) {
          if (isset($this->checked[$entity_type][$entity_id])) {
            unset($this->dependencies[$entity_type][$entity_id]);
          }
          else {
            $this->origins[$entity_type][$entity_id] = array(
              'type' => $this->entityType,
              'uuid' => reset(entity_get_uuid_by_id($this->entityType, array($this->entityId))),
            );
          }
        }
      }
      // Let other modules have their say.
      drupal_alter('deploy_entity_dependencies', $this->dependencies, $entity, $this->entityType);
      drupal_alter('deploy_entity_belongings', $this->belongings, $entity, $this->entityType);

      // Now mark this as checked.
      $this->checked[$this->entityType][$this->entityId] = TRUE;

      if (!empty($this->dependencies)) {
        return TRUE;
      }
    }
    return FALSE;
  }

  /**
   * Returns an iterator for the current entry.
   *
   * @return DeployIterator
   */
  public function getChildren() {
    $entities = array();
    $current = current($this->entities);
    if (is_array($current)) {
      $entities = $current;
    }

    if (!empty($this->dependencies) || !empty($this->belongings)) {
      // TODO: We need to merge with $entities from above in some smart way.
      $entities = $this->dependencies;
      // In an iterator, having children means that the current key itself
      // isn't a part of the entities. However, this isn't the case when doing
      // deployments. So we add the parent as a part of the entities.
      // And since children always should go first, we add the parent last.
      $entities[$this->entityType][] = $this->entityId;
      // TODO: Add belongings to the array.
      //foreach ($this->belongings as $entity_type => $entity_id) {
      //  $entities[$entity_type][] = $entity_id;
      //}
    }

    $iterator = new DeployIterator($entities, $this);
    $iterator->entityType = $this->entityType;
    return $iterator;
  }

  /**
   * Get the current entity formatted according to the OData protocol.
   *
   * @see http://www.odata.org/developers/protocols
   */
  public function current() {
    $current = current($this->entities);
    if (is_array($current)) {
      return $current;
    }

    // Build the entity according to the OData protocol.
    $uuid = reset(entity_get_uuid_by_id($this->entityType, array($this->entityId)));
    // Load the current entity with UUID.
    $entity = reset(entity_uuid_load($this->entityType, array($uuid)));
    // Add necessary metadata to the entity.
    $origin = FALSE;
    if (!empty($this->origins[$this->entityType][$this->entityId])) {
      $origin = array(
        '__deferred' => array(
          'uri' => $this->origins[$this->entityType][$this->entityId]['type'] . '/' . $this->origins[$this->entityType][$this->entityId]['uuid'],
        ),
      );
    }
    $entity->__metadata = array(
      'type' => $this->entityType,
      'uri' => $this->entityType . '/' . $uuid,
      'origin' => $origin,
    );
    // Let other modules have their say.
    drupal_alter('deploy_entity', $entity, $this->entityType);

    // Now mark this as traversed.
    $this->traversed[$this->entityType][$this->entityId] = TRUE;
    return $entity;
  }

  /**
   * Returns the key of the current element.
   */
  public function key() {
    return key($this->entities);
  }

  /**
   * Moves the current position to the next element.
   */
  public function next() {
    return next($this->entities);
  }

  /**
   * Rewinds the Iterator to the first element.
   */
  public function rewind() {
    reset($this->entities);
  }

  /**
   * Checks if current position is valid.
   *
   * @return boolean
   */
  public function valid() {
    $current = current($this->entities);
    if (!empty($current) && is_array($current)) {
      $this->entityType = key($this->entities);
      return TRUE;
    }
    elseif (!empty($current) && is_numeric($current) && isset($this->entityType) && !isset($this->traversed[$this->entityType][$current])) {
      $this->entityId = $current;
      return TRUE;
    }
    return FALSE;
  }
}

/**
 * Fake iterator that can be used when a DeployIterator is needed but we don't
 * want to check for dependencies.
 */
class DeployFakeIterator extends DeployIterator {
  public function hasChildren() {
    return FALSE;
  }
}
