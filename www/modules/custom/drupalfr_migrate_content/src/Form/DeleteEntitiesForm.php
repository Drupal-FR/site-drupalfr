<?php

namespace Drupal\drupalfr_migrate_content\Form;

use Drupal\Core\Form\FormBase;
use Drupal\Core\Form\FormStateInterface;
use Symfony\Component\DependencyInjection\ContainerInterface;

/**
 * Class EntitiesForm.
 */
class DeleteEntitiesForm extends FormBase
{

    private const DELETE_NUMBER_MAX = 300;

    /**
     * Drupal\Core\Entity\EntityTypeManagerInterface definition.
     *
     * @var \Drupal\Core\Entity\EntityTypeManagerInterface
     */
    protected $entityTypeManager;

    /**
     * Drupal\Core\Messenger\Messenger definition.
     *
     * @var \Drupal\Core\Messenger\Messenger
     */
    protected $messenger;

    /**
     * {@inheritdoc}
     */
    public static function create(ContainerInterface $container)
    {
        $instance = parent::create($container);
        $instance->entityTypeManager = $container->get('entity_type.manager');
        $instance->messenger = $container->get('messenger');

        return $instance;
    }

    /**
     * {@inheritdoc}
     */
    public function getFormId()
    {
        return 'entities_form';
    }

    /**
     * {@inheritdoc}
     */
    public function buildForm(array $form, FormStateInterface $form_state)
    {
        $options = $this->getNodesTypes();
        if(empty($options)) {
            return [
                '#type' => 'markup',
                '#markup' => $this->t('No entities existing.'),
            ];
        }

        $form['deletable_nodes'] = [
            '#type' => 'checkboxes',
            '#title' => $this->t('Nodes types to delete'),
            '#description' => $this->t('Choose nodes types and contents to delete'),
            '#options' => $options,
            '#required' => true,
            '#weight' => '0',
            ];

        // $users = $this->getUsersUids();

        /*if (count($users)>0) {
            $form['deletable_users'] = [
                '#type' => 'checkbox',
                '#title' => $this->t('Delete Users'),
                '#default_value' => false,
                '#required' => true,
                '#weight' => '0',
                ];
        }*/

        $form['submit'] = [
            '#type' => 'submit',
            '#value' => $this->t('Submit'),
            ];
        $this->getNodesTypes();

        return $form;
    }


    /*
     * {@inheritdoc}
     */
    public function submitForm(array &$form, FormStateInterface $form_state)
    {
        // Display result.
        $selection = $form_state->getValue('deletable_nodes');
        $selectionUsers = $form_state->getValue('deletable_users');
        $selections = [];
        foreach($selection as $sel) {
            if (0!==$sel) {
                $selections[] = $sel;
            }

        }
        $this->deleteNodesByTypes($selections);

    }

    private function getUsersUids(): array {
        $uids = \Drupal::database()->select('users','u')
                ->fields('u',['uid'])
                ->execute()
                ->fetchAllKeyed();
                dump($uids);
        return $uids;
    }

    private function getNodesTypes()
    {
        $query = $this->entityTypeManager->getStorage('node')->getAggregateQuery()
            ->groupBy('type');
        $result = $query->execute();
        $types = [];
        foreach($result as $res) {
            $types[$res['type']] = $res['type'];
        }
        foreach($types as $key => $type) {
            $res = \Drupal::database()->select('node','n')
                ->fields('n',['nid'])
                ->condition('type',$key)
                ->execute()
                ->fetchAll();
            $types[$key] = sprintf('%s (%s nodes)', $key, count($res));
        }
        return $types;
    }

    private function deleteNodesByTypes($types) {
        $res = \Drupal::database()->select('node','n')
        ->fields('n',['nid'])
        ->condition('type',$types,'in')
        ->execute()
        ->fetchAllKeyed();
        $nids = array_keys($res);

        $batch = [
            'title' => $this->t('Deleting nodes'),
            'init_message' => $this->t('Get nodes to delete.'),
            'progress_message' => $this->t('Completed step @current of @total.'),
            'file' => drupal_get_path('module', 'drupalfr_migrate_content') . '/includes/nodes.batch.inc',
            'finished' => ['finishBatch', $nids],
        ];
        while (count($nids) > 0) {
            $numberToExtract = count($nids) >= self::DELETE_NUMBER_MAX ? self::DELETE_NUMBER_MAX : count($nids);
            $extract = array_slice($nids, 0, $numberToExtract);
            array_splice($nids, 0, $numberToExtract);
            $batch['operations'][] = ['deleteNodesMultiple', [$extract]];
            $batch['error_message'] = $this->t('Error deleting nodes.');
        }
        batch_set($batch);
    }
}
