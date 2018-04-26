/*
 * twoskip
 *
 * twoskip is free software; you can redistribute it and/or modify
 * it under the terms of the MIT license. See LICENSE for details.
 *
 * Copyright (c) 2017 Partha Susarla <mail@spartha.org>
 */

#include "btree.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define NUMRECS 20

int main(int argc __attribute__((unused)), char **argv __attribute__((unused)))
{
        struct btree *tree = NULL;
        struct record *recs[NUMRECS];
        int j, ret;
        btree_iter_t iter = { 0 }, iter1 = { 0 };

        tree = btree_new(NULL, NULL);

        for (j = 0; j < NUMRECS; j++) {
                char key[10] = { 0 };
                char val[10] = { 0 };
                sprintf(key, "key%d", j);
                sprintf(val, "val%d", j);
                recs[j] = record_new((unsigned char *)key, strlen(key),
                                     (unsigned char *)val, strlen(val));
                if (btree_insert(tree, recs[j]) != BTREE_OK) {
                        fprintf(stderr, "btree_insert didn't work for %s\n", key);
                        goto done;
                }
        }

        printf("------------------------\n");
        btree_print_node_data(tree, NULL);
        printf("------------------------\n");
        {
                struct record *trec = record_new((unsigned char *)"key2", strlen("key2"),
                                                 (unsigned char *)"newval2", strlen("newval2"));
                ret = btree_insert(tree, trec);
                if (ret == BTREE_DUPLICATE) {
                        fprintf(stderr, "duplicate key: key2, replacing\n");
                        ret = btree_replace(tree, trec);
                }
                if (ret != BTREE_OK) {
                        fprintf(stderr, "failed inserting key2\n");
                }
        }

        btree_print_node_data(tree, NULL);
        printf("------------------------\n");

        printf("------------------------\n");
        {
                struct record *trec = record_new((unsigned char *)"key199", strlen("key199"),
                                                 (unsigned char *)"val199", strlen("val199"));
                ret = btree_insert(tree, trec);
                if (ret != BTREE_OK) {
                        fprintf(stderr, "failed inserting key199\n");
                }
        }

        btree_print_node_data(tree, NULL);
        printf("------------------------\n");

        if (btree_find(tree, (unsigned char *)"key2", strlen("key2"), iter)) {
                size_t i;
                printf("Record found:\n");
                printf("key: ");
                for (i = 0; i < iter->record->keylen; i++) {
                        printf("%c", iter->record->key[i]);
                }
                printf("\n");
                printf("val: ");
                for (i = 0; i < iter->record->vallen; i++) {
                        printf("%c", iter->record->val[i]);
                }
                printf("\n");
        } else {
                printf("Record with key `key2` NOT FOUND.\n");
                size_t i;
                printf("Record AFTER:\n");
                printf("key: ");
                for (i = 0; i < iter->record->keylen; i++) {
                        printf("%c", iter->record->key[i]);
                }
                printf("\n");
                printf("val: ");
                for (i = 0; i < iter->record->vallen; i++) {
                        printf("%c", iter->record->val[i]);
                }
                printf("\n");
        }

        /* print using an iterator */
        printf("--- iterator print ---\n");
        for (btree_begin(tree, iter1); btree_next(iter1);) {
                size_t i;
                for (i = 0; i < iter1->record->keylen; i++) {
                        printf("%c", iter1->record->key[i]);
                }
                printf(" : ");
                for (i = 0; i < iter1->record->vallen; i++) {
                        printf("%c", iter1->record->val[i]);
                }
                printf("\n");
        }
        printf("-----------------------\n");
done:
        btree_free(tree);

        exit(EXIT_SUCCESS);
}