<template>
    <v-app>
        <v-navigation-drawer
                app
                clipped
                mini-variant
                expand-on-hover>
            <v-list dense>
                <v-list-item link v-for="item in drawerMenu"
                             :to="item.link"
                             :key="item.title">
                    <v-list-item-action>
                        <v-icon>{{item.icon}}</v-icon>
                    </v-list-item-action>
                    <v-list-item-content>
                        <v-list-item-title>{{item.title}}</v-list-item-title>
                    </v-list-item-content>
                </v-list-item>
            </v-list>
        </v-navigation-drawer>

        <v-main>
            <v-container fluid>
                <v-row>
                    <v-col cols="12">
                        <v-row justify="center">
                            <transition name="fade">
                                <router-view/>
                            </transition>
                        </v-row>
                    </v-col>
                </v-row>
            </v-container>
        </v-main>
        <v-footer>
            <v-layout justify-center>
                {{this.remainingTime}}
            </v-layout>
        </v-footer>
    </v-app>
</template>

<script>
    export default {
        name: "MainApp",
        data() {
            return {
                drawerMenu: [
                    {
                        title: 'Úvod', icon: 'mdi-view-dashboard', link: {name: 'Home'}
                    },
                    {
                        title: 'Drátový modul', icon: 'mdi-cable-data', link: {name: 'Manual', params: {'moduleName': 'cables', 'page': 1}}
                    },
                    {
                        title: 'Číselný modul', icon: 'mdi-keyboard', link: {name: 'Manual', params: {'moduleName': 'keyboard', 'page': 1}}
                    }
                ]
            }
        },
        mounted() {
            this.initWs()
            this.downloadPuzzles()
        },
        methods: {
            downloadPuzzles: function () {
                this.ajaxGet("puzzles.json", {}, 15000).then(r => {
                    this.puzzleModules = r;
                })
            }
        },
    }
</script>

<style scoped lang="scss">
    .content {
        padding: 10px;
    }
</style>
