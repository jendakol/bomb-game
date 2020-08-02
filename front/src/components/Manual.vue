<template>
    <v-container width="1000" outlined raised fill-height>
        <v-row>
            <v-container fluid>
                <v-card class="pa-5">
                    <v-card-title>{{this.moduleNames[this.$route.params.moduleName]}}</v-card-title>
                    <v-card-text v-if="this.pagiPage === 1">
                        {{ this.puzzleModule.intro }}
                    </v-card-text>
                    <v-card-text v-else>
                        {{ this.puzzleModule.puzzles[this.pagiPage - 2] }}
                    </v-card-text>
                </v-card>
            </v-container>
        </v-row>
        <v-row>
            <v-layout align-center justify-center>
                <v-footer fluid>
                    <v-pagination
                            v-model="pagiPage"
                            @input="changePage"
                            :length="this.puzzleModule.puzzles.length + 1"
                    />
                </v-footer>
            </v-layout>
        </v-row>
    </v-container>
</template>

<script>
    export default {
        name: 'Manual',
        components: {},
        data() {
            return {
                pagiPage: Number.parseInt(this.$route.params.page),
                moduleNames: {"cables": "Drátový modul", "keyboard": "Číselný modul"},
            }
        },
        methods: {
            changePage: function () {
                this.$router.push({name: 'Manual', params: {'moduleName': this.$route.params.moduleName, 'page': this.pagiPage}});
            }
        },
        computed: {
            puzzleModule: {
                get: function () {
                    return this.puzzleModules[this.$route.params.moduleName]
                }
            },
        }
    }
</script>
