import Manual from "./components/Manual";
import ModuleSelect from "./components/ModuleSelect";
import Admin from "./components/Admin";

const routes = [
    {name: "Root", path: '/', redirect: '/home'},
    {name: 'Home', path: '/home', component: ModuleSelect},
    {name: 'Admin', path: '/admin/:pass', component: Admin},
    {name: 'Manual', path: '/manual/:moduleName/:page', component: Manual},
    {path: '*', redirect: '/home'},
]

export default routes
