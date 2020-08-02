<template>
    <div>
        404 NOT FOUND<br/>
        Go to <router-link :to="{name: 'Home'}">home!</router-link>
    </div>
</template>

<script>
    export default {
        name: 'NotFound',
    }
</script>
