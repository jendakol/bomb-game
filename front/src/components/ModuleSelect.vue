<template>
    <v-app>
        <v-content>
            <v-container fluid>
                <h1 style="text-align: center">Bomba BMBSTV2020WQ</h1>
                <v-layout justify-center class="pt-5">
                    <v-list rounded>
                        <v-list-item class="ma-3" :to="{name: 'Manual', params: {'moduleName': 'cables', 'page': 1}}">
                            <v-list-item-icon>
                                <v-icon>mdi-cable-data</v-icon>
                            </v-list-item-icon>
                            <v-list-item-title>Drátový modul</v-list-item-title>
                        </v-list-item>
                        <v-list-item class="ma-3" :to="{name: 'Manual', params: {'moduleName': 'keyboard', 'page': 1}}">
                            <v-list-item-icon>
                                <v-icon>mdi-keyboard</v-icon>
                            </v-list-item-icon>
                            <v-list-item-title>Číselný modul</v-list-item-title>
                        </v-list-item>
                    </v-list>
                </v-layout>
            </v-container>
        </v-content>
    </v-app>
</template>

<script>
    export default {
        name: 'LoginForm',
        data() {
            return {}
        },
        methods: {
            select: function (module) {
                console.log(module)
            }
        }
    }
</script>
