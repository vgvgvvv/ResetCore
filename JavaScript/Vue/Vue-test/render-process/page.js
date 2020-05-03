
new Vue({
    name: 'notebook',

    el:'#notebook',
    
    data(){
        return {
            content:'asdasdsad'
        }
    },

    computed: {
        notePreview(){
            return marked(this.content)
        }
    }
})