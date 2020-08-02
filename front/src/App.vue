<template>
    <v-app>
        <ManualSelect v-if="!this.isManualSelected"/>
        <MainApp v-else/>

        <vue-snotify/>
    </v-app>

</template>

<style lang="sass">
    @import 'fonts.css'
    /*@import '../node_modules/material-design-icons/iconfont/material-icons.css'*/
</style>

<script>
    import ManualSelect from './components/ManualSelect.vue';
    import MainApp from './components/MainApp.vue';

    export default {
        components: {
            MainApp,
            ManualSelect
        },
        mounted() {
        },
        data() {
            return {}
        },
        methods: {},
    }
</script>
