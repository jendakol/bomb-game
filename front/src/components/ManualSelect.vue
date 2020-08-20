<template>
    <v-app>
        <v-content>
            <v-container fluid fill-height>
                <v-layout align-center justify-center>
                    <v-flex xs12 sm8 md4>
                        <v-card class="elevation-12">
                            <v-card-title>Vyberte verzi vaší bomby:</v-card-title>
                            <v-card-text>
                                <v-select
                                        :items="items"
                                        v-model="selectedVersion"
                                        @change="selected"
                                        outlined
                                ></v-select>
                            </v-card-text>

                        </v-card>
                    </v-flex>
                </v-layout>
            </v-container>
        </v-content>
    </v-app>
</template>

<script>
    import {SnotifyPosition} from "vue-snotify";

    export default {
        name: 'LoginForm',
        data() {
            return {
                items: ['BMBSTV2020WOQ', 'BMBSTV2020WQ'],
                selectedVersion: null,
            }
        },
        methods: {
            selected: function () {
                if (this.selectedVersion === this.items[1]) {
                    this.isManualSelected = true;
                } else {
                    let toast = this.$snotify.error("Požadovaný manuál nebyl nalezen!\nBude zobrazen manuál k příbuznému modelu BMBSTV2020WQ, který je psán v hádankách.", {
                        timeout: 8000,
                        position: SnotifyPosition.centerCenter,
                        backdrop: 0.9,
                    });

                    toast.on('hidden', () => {
                        this.isManualSelected = true;
                    })
                }
            }
        }
    }
</script>
