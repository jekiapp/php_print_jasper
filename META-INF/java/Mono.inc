<?php
# Copyright (C) 2003-2009 Jost Boekemeier.
# Distributed under the MIT license, see Options.inc for details. 
# Customization examples:
# define ("MONO_SERVLET", "User");
# define ("MONO_SERVLET", "/MyWebApp/MonoBridge.phpmonobridge");
# define ("MONO_HOSTS", "127.0.0.1:8787");
# define ("MONO_PERSISTENT_SERVLET_CONNECTIONS", true);
# define ("MONO_PREFER_VALUES", 1);
# define ("MONO_DEBUG", true);
# define ("MONO_PIPE_DIR", "/dev/shm");
if(!function_exists("mono_get_base")) {
$version = phpversion();
if ((version_compare("5.3.0", $version, ">"))) {
1E512; 
if ((version_compare("5.1.2", $version, ">"))) {
$msg = "<br><strong>PHP $version too old.</strong><br>\nPlease set the path to
a PHP 5.1.x executable, see php_exec in the WEB-INF/web.xml";
die($msg);
}
}
function mono_get_base() {
$ar = get_required_files();
$arLen = sizeof($ar);
if($arLen>0) {
$thiz = $ar[$arLen-1];
return dirname($thiz);
} else {
return "mono/";
}
}
$MONO_BASE=mono_get_base();
class mono_RuntimeException extends Exception {};
class mono_IOException extends Exception {};
class mono_ConnectException extends mono_IOException {};
class mono_IllegalStateException extends mono_RuntimeException {};
class mono_IllegalArgumentException extends mono_RuntimeException {
function __construct($ob) {
parent::__construct("illegal argument: ".gettype($ob));
}
};
function mono_autoload_function5($x) {
$str=str_replace("_", ".", $x);
$client=__monoproxy_Client_getClient();
if(!($client->invokeMethod(0, "typeExists", array($str)))) return false;
$instance = "class ${x} extends Mono {".
"static function type(\$sub=null){if(\$sub) \$sub='\$'.\$sub; return mono('${str}'.\"\$sub\");}".
'function __construct() {$args = func_get_args();'.
'array_unshift($args, '."'$str'".'); parent::__construct($args);}}';
eval ("$instance");
return true;
}
function mono_autoload_function($x) {
$idx = strrpos($x, "\\"); if (!$idx) return mono_autoload_function5($x);
$str=str_replace("\\", ".", $x);
$client=__monoproxy_Client_getClient();
if(!($client->invokeMethod(0, "typeExists", array($str)))) return false;
$package = substr($x,0, $idx);
$name = substr($x, 1+$idx);
$instance = "namespace $package; class ${name} extends \\Mono {".
"static function type(\$sub=null){if(\$sub) \$sub='\$'.\$sub;return \\mono('${str}'.\"\$sub\");}".
'function __construct() {$args = func_get_args();'.
'array_unshift($args, '."'$str'".'); parent::__construct($args);}}';
eval ("$instance");
return true;
}
function mono_autoload($libs=null) {
static $once = false;
if($once)
throw new mono_IllegalStateException("mono_autoload called more than once");
$once = true;
mono_require($libs);
if(function_exists("spl_autoload_register")) {
spl_autoload_register("mono_autoload_function");
} else {
function __autoload($x) {
return mono_autoload_function($x);
}
}
}
function Mono($name) {
static $classMap = array();
if(array_key_exists($name, $classMap)) return $classMap[$name];
return $classMap[$name]=new MonoClass($name);
}
function mono_get_closure() {return mono_closure_array(func_get_args());}
function mono_wrap() {return mono_closure_array(func_get_args());}
function mono_get_values($arg) { return mono_values($arg); }
function mono_get_session() {return mono_session_array(func_get_args());}
function mono_get_context() {return mono_context(); }
function mono_get_server_name() { return mono_server_name(); }
function mono_isnull($value) { return is_null (mono_values ($value)); }
function mono_is_null($value) { return is_null (mono_values ($value)); }
function mono_istrue($value) { return (boolean)(mono_values ($value)); }
function mono_is_true($value) { return (boolean)(mono_values ($value)); }
function mono_isfalse($value) { return !(mono_values ($value)); }
function mono_is_false($value) { return !(mono_values ($value)); }
function mono_set_encoding($enc) { return mono_set_file_encoding ($enc); }
function mono_set_library_path($arg) { return mono_require($arg); }
function mono_defineHostFromInitialQuery($mono_base) {
if($mono_base!="mono/") {
$url = parse_url($mono_base);
if(isset($url["scheme"]) && ($url["scheme"]=="http")) {
$host = $url["host"];
$port = $url["port"];
$path = $url["path"];
define ("MONO_HOSTS", "$host:$port");
$dir = dirname($path);
define ("MONO_SERVLET", "$dir/MonoBridge.phpmonobridge"); 
return true;
}
}
return false;
}
define ("MONO_PEAR_VERSION", "5.4.3");
if(!defined("MONO_SEND_SIZE"))
define("MONO_SEND_SIZE", 8192);
if(!defined("MONO_RECV_SIZE"))
define("MONO_RECV_SIZE", 8192);
if(!defined("MONO_HOSTS")) {
if(!mono_defineHostFromInitialQuery($MONO_BASE)) {
if ($ini=get_cfg_var("mono.hosts")) define("MONO_HOSTS", $ini);
else define("MONO_HOSTS", "127.0.0.1:8080"); 
}
}
if(!defined("MONO_SERVLET")) {
if (!(($ini=get_cfg_var("mono.servlet"))===false)) define("MONO_SERVLET", $ini);
else define("MONO_SERVLET", 1); // "Off"|0, "On"|1 or "User"
}
if(!defined("MONO_LOG_LEVEL"))
if (!(($ini=get_cfg_var("mono.log_level"))===false)) define("MONO_LOG_LEVEL", (int)$ini);
else define("MONO_LOG_LEVEL", null); 
if(!defined("MONO_PIPE_DIR")) {
if ($ini=get_cfg_var("mono.pipe_dir")) define("MONO_PIPE_DIR", $ini);
else
if (file_exists ("/dev/shm")) define("MONO_PIPE_DIR", "/dev/shm" );
else if (PHP_OS==='SunOS') define("MONO_PIPE_DIR", "/tmp"); # shared memory on Solaris
else define("MONO_PIPE_DIR", null);
}
if (!defined("MONO_PREFER_VALUES"))
if ($ini=get_cfg_var("mono.prefer_values")) define("MONO_PREFER_VALUES", $ini);
else define("MONO_PREFER_VALUES", 0);
if(!defined("MONO_DEBUG"))
if ($ini=get_cfg_var("mono.debug")) define("MONO_DEBUG", $ini);
else define("MONO_DEBUG", false);
if(!defined("MONO_PERSISTENT_SERVLET_CONNECTIONS"))
if ($ini=get_cfg_var("mono.persistent_servlet_connections")) define("MONO_PERSISTENT_SERVLET_CONNECTIONS", $ini);
else define("MONO_PERSISTENT_SERVLET_CONNECTIONS", false);
class mono_SimpleFactory {
public $client;
function mono_SimpleFactory($client) {
$this->client = $client;
}
function getProxy($result, $signature, $exception, $wrap) {
if (false) { $signature = $signature; $wrap = $wrap; $exception = $exception; }
return $result;
}
function checkResult($result) {
if (false) $result = $result;
}
}
class mono_ProxyFactory extends mono_SimpleFactory {
function mono_ProxyFactory($client) {
parent::mono_SimpleFactory($client);
}
function create($result, $signature) {
return new mono_MonoProxy($result, $signature);
}
function createInternal($proxy) {
return new mono_InternalMono($proxy);
}
function getProxy($result, $signature, $exception, $wrap) {
if (false) { $exception = $exception; }
$proxy = $this->create($result, $signature);
if($wrap) $proxy = $this->createInternal($proxy);
return $proxy;
}
}
class mono_ArrayProxyFactory extends mono_ProxyFactory {
function mono_ArrayProxyFactory($client) {
parent::mono_ProxyFactory($client);
}
function create($result, $signature) {
return new mono_ArrayProxy($result, $signature);
}
}
class mono_IteratorProxyFactory extends mono_ProxyFactory {
function mono_IteratorProxyFactory($client) {
parent::mono_ProxyFactory($client);
}
function create($result, $signature) {
return new mono_IteratorProxy($result, $signature);
}
}
class mono_ExceptionProxyFactory extends mono_SimpleFactory {
function mono_ExceptionProxyFactory($client) {
parent::mono_SimpleFactory($client);
}
function create($result, $signature) {
return new mono_ExceptionProxy($result, $signature);
}
function getProxy($result, $signature, $exception, $wrap) {
$proxy = $this->create($result, $signature);
if($wrap) $proxy = new mono_InternalException($proxy, $exception);
return $proxy;
}
}
class mono_ThrowExceptionProxyFactory extends mono_ExceptionProxyFactory {
function mono_ThrowExceptionProxyFactory($client) {
parent::mono_ExceptionProxyFactory($client);
}
function getProxy($result, $signature, $exception, $wrap) {
if (false) { $wrap = $wrap; }
$proxy = $this->create($result, $signature);
$proxy = new mono_InternalException($proxy, $exception);
return $proxy;
}
function checkResult($result) {
throw $result;
}
}
class mono_CacheEntry {
public $fmt, $signature, $factory, $mono;
public $resultVoid;
function mono_CacheEntry($fmt, $signature, $factory, $resultVoid) {
$this->fmt = $fmt;
$this->signature = $signature;
$this->factory = $factory;
$this->resultVoid = $resultVoid;
}
}
class mono_Arg {
public $client;
public $exception; 
public $factory, $val;
public $signature; 
function mono_Arg($client) {
$this->client = $client;
$this->factory = $client->simpleFactory;
}
function linkResult(&$val) {
$this->val = &$val;
}
function setResult($val) {
$this->val = &$val;
}
function getResult($wrap) {
$rc = $this->factory->getProxy($this->val, $this->signature, $this->exception, $wrap);
$factory = $this->factory;
$this->factory = $this->client->simpleFactory;
$factory->checkResult($rc);
return $rc;
}
function setFactory($factory) {
$this->factory = $factory;
}
function setException($string) {
$this->exception = $string;
}
function setVoidSignature() {
$this->signature = "@V";
$key = $this->client->currentCacheKey;
if($key && $key[0]!='~') { 
$this->client->currentArgumentsFormat[6]="3";
if(MONO_DEBUG) {
echo "ignore further results:"; echo "\n";
}
if(MONO_DEBUG) {
echo "updating cache $key, argformat: {$this->client->currentArgumentsFormat}, classType: {$this->signature}\n";
}
$cacheEntry = new mono_CacheEntry($this->client->currentArgumentsFormat, $this->signature, $this->factory, true);
$this->client->methodCache[$key]=$cacheEntry;
}
}
function setSignature($signature) {
$this->signature = $signature;
$key = $this->client->currentCacheKey;
if($key && $key[0]!='~') { 
if(MONO_DEBUG) {
echo "updating cache $key, argformat: {$this->client->currentArgumentsFormat}, classType: {$this->signature}\n";
}
$cacheEntry = new mono_CacheEntry($this->client->currentArgumentsFormat, $this->signature, $this->factory, false);
$this->client->methodCache[$key]=$cacheEntry;
}
}
}
class mono_CompositeArg extends mono_Arg {
public $parentArg;
public $idx; 
public $type; 
public $counter;
function mono_CompositeArg($client, $type) {
parent::mono_Arg($client);
$this->type = $type;
$this->val = array();
$this->counter = 0;
}
function setNextIndex() {
$this->idx = $this->counter++;
}
function setIndex($val) {
$this->idx = $val;
}
function linkResult(&$val) {
$this->val[$this->idx]=&$val;
}
function setResult($val) {
$this->val[$this->idx]=$this->factory->getProxy($val, $this->signature, $this->exception, true);
$this->factory = $this->client->simpleFactory;
}
}
class mono_ApplyArg extends mono_CompositeArg {
public $m, $p, $v, $n; 
function mono_ApplyArg($client, $type, $m, $p, $v, $n) {
parent::mono_CompositeArg($client, $type);
$this->m = $m;
$this->p = $p;
$this->v = $v;
$this->n = $n;
}
}
class mono_Client 
 {
public $RUNTIME;
public $result, $exception;
public $parser;
public $simpleArg, $compositeArg;
public $simpleFactory,
$proxyFactory, $iteratorProxyFacroty,
$arrayProxyFactory, $exceptionProxyFactory, $throwExceptionProxyFactory;
public $arg;
public $asyncCtx, $cancelProxyCreationCounter;
public $globalRef;
public $stack;
public $defaultCache = array(), $asyncCache = array(), $methodCache;
public $isAsync = 0;
public $currentCacheKey, $currentArgumentsFormat;
public $cachedMonoPrototype;
public $sendBuffer, $preparedToSendBuffer;
function mono_Client() {
$this->RUNTIME = array();
$this->RUNTIME["NOTICE"]='***USE echo mono_inspect(jVal) OR print_r(mono_values(jVal)) TO SEE THE CONTENTS OF THIS MONO OBJECT!***';
if(MONO_PIPE_DIR && function_exists("posix_mkfifo"))
$this->RUNTIME['PIPE_DIR']=MONO_PIPE_DIR;
else
$this->RUNTIME['PIPE_DIR']=null;
$this->parser = new mono_Parser($this);
$this->protocol = new mono_Protocol($this);
$this->simpleFactory = new mono_SimpleFactory($this);
$this->proxyFactory = new mono_ProxyFactory($this);
$this->arrayProxyFactory = new mono_ArrayProxyFactory($this);
$this->iteratorProxyFactory = new mono_IteratorProxyFactory($this);
$this->exceptionProxyFactory = new mono_ExceptionProxyFactory($this);
$this->throwExceptionProxyFactory = new mono_ThrowExceptionProxyFactory($this);
$this->cachedMonoPrototype=new mono_MonoProxyProxy($this);
$this->simpleArg = new mono_Arg($this);
$this->globalRef = new mono_GlobalRef();
$this->asyncCtx = $this->cancelProxyCreationCounter = 0;
$this->methodCache = $this->defaultCache;
}
function read($size) {
return $this->protocol->read($size);
}
function setDefaultHandler() {
$this->methodCache = $this->defaultCache;
}
function setAsyncHandler() {
$this->methodCache = $this->asyncCache;
}
function handleRequests() {
do {
$tail_call = false;
$this->stack=array($this->arg=$this->simpleArg);
$this->idx = 0;
$this->parser->parse();
if((count($this->stack)) > 1) {
$arg = array_pop($this->stack);
$this->apply($arg);
$tail_call = 1; 
} else {
$tail_call = 0;
}
$this->stack=null;
} while($tail_call);
return 1;
}
function getWrappedResult($wrap) {
return $this->simpleArg->getResult($wrap);
}
function getInternalResult() {
return $this->getWrappedResult(false);
}
function getResult() {
return $this->getWrappedResult(true);
}
function getProxyFactory($type) {
switch($type[0]) {
case 'E':
$factory = $this->exceptionProxyFactory;
break;
case 'C':
$factory = $this->iteratorProxyFactory;
break;
case 'A':
$factory = $this->arrayProxyFactory;
break;
case 'O':
$factory = $this->proxyFactory;
}
return $factory;
}
function link(&$arg, &$newArg) {
$arg->linkResult($newArg->val);
$newArg->parentArg = $arg;
}
function getExact($str) {
return hexdec($str);
}
function getInexact($str) {
$val = null;
sscanf($str, "%e", $val);
return $val;
}
function begin($name, $st) {
$arg = $this->arg;
switch($name[0]) {
case 'A': 
$object = $this->globalRef->get($this->getExact($st['v']));
$newArg = new mono_ApplyArg($this, 'A',
$this->parser->getData($st['m']),
$this->parser->getData($st['p']),
$object,
$this->getExact($st['n']));
$this->link($arg, $newArg);
array_push($this->stack, $this->arg = $newArg);
break;
case 'X':
$newArg = new mono_CompositeArg($this, $st['t']);
$this->link($arg, $newArg);
array_push($this->stack, $this->arg = $newArg);
break;
case 'P':
if($arg->type=='H') { 
$s = $st['t'];
if(MONO_DEBUG) {echo "setresult prepare hash:"; echo sprintf("%s", $st['t']); echo "\n";}
if($s[0]=='N') { 
$arg->setIndex($this->getExact($st['v']));
if(MONO_DEBUG) {echo "setresult array: index:"; echo sprintf("%s", $st['v']); echo "\n";}
} else {
$arg->setIndex($this->parser->getData($st['v']));
if(MONO_DEBUG) {echo "setresult hash: index:"; echo sprintf("%s", $this->parser->getData($st['v'])); echo "\n";}
}
} else { 
$arg->setNextIndex();
}
break;
case 'S':
$arg->setResult($this->parser->getData($st['v']));
if(MONO_DEBUG) {echo "setresult string:"; echo sprintf("%s", $this->parser->getData($st['v'])); echo "\n";}
break;
case 'B':
$s=$st['v'];
$arg->setResult($s[0]=='T');
if(MONO_DEBUG) {echo "setresult bool:"; echo sprintf("%s", $st['v']); echo "\n";}
break;
case 'L': 
$sign = $st['p'];
$val = $this->getExact($st['v']);
if($sign[0]=='A') $val*=-1;
$arg->setResult($val);
if(MONO_DEBUG) {echo "setresult long:"; echo sprintf("%s, sign: %s", $st['v'], $st['p']); echo "\n";}
break;
case 'D':
$arg->setResult($this->getInexact($st['v']));
if(MONO_DEBUG) {echo "setresult double:"; echo sprintf("%s", $st['v']); echo "\n";}
break;
case 'V':
if ($st['n']!='T') {
if(MONO_DEBUG) {echo "setresult VOID:"; echo "\n";}
$arg->setVoidSignature();
}
case 'N':
$arg->setResult(null);
if(MONO_DEBUG) {echo "setresult null\n"; }
break;
case 'F':
if(MONO_DEBUG) {echo "comm. end\n"; }
break;
case 'O':
$arg->setFactory($this->getProxyFactory($st['p']));
$arg->setResult($this->asyncCtx=$this->getExact($st['v']));
if($st['n']!='T') $arg->setSignature($st['m']);
if(MONO_DEBUG) {echo "setresult object:"; echo sprintf("%x", $this->asyncCtx); echo "\n";}
break;
case 'E':
$arg->setFactory($this->throwExceptionProxyFactory);
if(MONO_DEBUG) {echo "setresult exception:"; echo sprintf("%x", $this->asyncCtx); echo "\n";}
$arg->setException($this->parser->getData($st['m']));
$arg->setResult($this->asyncCtx=$this->getExact($st['v']));
break;
default:
$this->parser->parserError();
}
}
function end($name) {
switch($name[0]) {
case 'X':
$frame = array_pop($this->stack);
$this->arg = $frame->parentArg;
break;
}
}
function createParserString() {
return new mono_ParserString();
}
function writeArg($arg) {
if(is_string($arg)) {
$this->protocol->writeString($arg);
} else if(is_object($arg)) {
if (!$arg instanceof mono_MonoType) throw new mono_IllegalArgumentException($arg);
$this->protocol->writeObject($arg->__mono);
return $arg;
} else if(is_null($arg)) {
$this->protocol->writeObject(null);
} else if(is_bool($arg)) {
$this->protocol->writeBoolean($arg);
} else if(is_integer($arg)) {
$this->protocol->writeLong($arg);
} else if(is_float($arg)) {
$this->protocol->writeDouble($arg);
} else if(is_array($arg)) {
$wrote_begin=false;
foreach($arg as $key=>$val) {
if(is_string($key)) {
if(!$wrote_begin) {
$wrote_begin=1;
$this->protocol->writeCompositeBegin_h();
}
$this->protocol->writePairBegin_s($key);
$this->writeArg($val);
$this->protocol->writePairEnd();
} else {
if(!$wrote_begin) {
$wrote_begin=1;
$this->protocol->writeCompositeBegin_h();
}
$this->protocol->writePairBegin_n($key);
$this->writeArg($val);
$this->protocol->writePairEnd();
}
}
if(!$wrote_begin) {
$this->protocol->writeCompositeBegin_a();
}
$this->protocol->writeCompositeEnd();
}
return null;
}
function writeArgs($args) {
$n = count($args);
for($i=0; $i<$n; $i++) {
$this->writeArg($args[$i]);
}
}
function createObject($name, $args) {
$this->protocol->createObjectBegin($name);
$this->writeArgs($args);
$this->protocol->createObjectEnd();
$val = $this->getInternalResult();
return $val;
}
function referenceObject($name, $args) {
$this->protocol->referenceBegin($name);
$this->writeArgs($args);
$this->protocol->referenceEnd();
$val = $this->getInternalResult();
return $val;
}
function getProperty($object, $property) {
$this->protocol->propertyAccessBegin($object, $property);
$this->protocol->propertyAccessEnd();
return $this->getResult();
}
function setProperty($object, $property, $arg) {
$this->protocol->propertyAccessBegin($object, $property);
$this->writeArg($arg);
$this->protocol->propertyAccessEnd();
$this->getResult();
}
function invokeMethod($object, $method, $args) {
$this->protocol->invokeBegin($object, $method);
$this->writeArgs($args);
$this->protocol->invokeEnd();
$val = $this->getResult();
return $val;
}
function unref($object) {
if (isset($this->protocol)) $this->protocol->writeUnref($object);
}
function apply($arg) {
$name = $arg->p;
$object = $arg->v;
$ob = $object ? array(&$object, $name) : $name;
$isAsync = $this->isAsync;
$methodCache = $this->methodCache;
$currentArgumentsFormat = $this->currentArgumentsFormat;
try {
$res = $arg->getResult(true);
if((!$object && !function_exists($name)) || ($object && !method_exists($object, $name))) throw new MonoException("mono.lang.NoSuchMethodError", "$name");
$res = call_user_func_array($ob, $res);
if (is_object($res) && !($res instanceof mono_MonoType)) {
trigger_error("object returned from $name() is not a Mono object", E_USER_WARNING);
$this->protocol->invokeBegin(0, "makeClosure");
$this->protocol->writeULong($this->globalRef->add($res)); // proper PHP "long" -> Mono 64 bit value conversion
$this->protocol->invokeEnd();
$res = $this->getResult();
}
$this->protocol->resultBegin();
$this->writeArg($res);
$this->protocol->resultEnd();
} catch (MonoException $e) {
$trace = $e->getTraceAsString();
$this->protocol->resultBegin();
$this->protocol->writeException($e->__mono, $trace);
$this->protocol->resultEnd();
} catch(Exception $ex) {
die ($ex);
}
$this->isAsync = $isAsync;
$this->methodCache = $methodCache;
$this->currentArgumentsFormat = $currentArgumentsFormat;
}
function cast($object, $type) {
switch($type[0]) {
case 'S': case 's':
return $this->invokeMethod(0, "castToString", array($object));
case 'B': case 'b':
return $this->invokeMethod(0, "castToBoolean", array($object));
case 'L': case 'I': case 'l': case 'i':
return $this->invokeMethod(0, "castToExact", array($object));
case 'D': case 'd': case 'F': case 'f':
return $this->invokeMethod(0, "castToInExact", array($object));
case 'N': case 'n':
return null;
case 'A': case 'a':
return $this->invokeMethod(0, "castToArray", array($object));
case 'O': case 'o': 
return $object;
default:
throw new mono_RuntimeException("$type illegal");
}
}
function getContext() {
static $cache = null;
if ($cache) return $cache;
return $cache = $this->invokeMethod(0, "getContext", array());
}
function getSession($args) {
static $cache = null;
if ($cache) {
trigger_error("mono_session() should be called once at the beginning of the script", E_USER_WARNING);
return $cache;
}
return $cache = $this->invokeMethod(0, "getSession", $args);
}
function getServerName() {
static $cache = null;
if ($cache) return $cache;
return $cache = $this->protocol->getServerName();
}
}
function mono_shutdown() {
global $mono_initialized;
if (!$mono_initialized) return;
$client = __monoproxy_Client_getClient();
if(MONO_DEBUG) echo "the client destroyed\n";
if (!isset($client->protocol)) return;
if ($client->preparedToSendBuffer)
$client->sendBuffer.=$client->preparedToSendBuffer;
$client->sendBuffer.=$client->protocol->getKeepAlive();
$client->protocol->flush();
$client->protocol->keepAlive();
}
register_shutdown_function("mono_shutdown");
class mono_GlobalRef {
public $map;
function mono_GlobalRef() {
$this->map = array();
}
function add($object) {
if(is_null($object)) return 0;
return array_push($this->map, $object);
}
function get($id) {
if(!$id) return 0;
return $this->map[--$id];
}
}
class mono_NativeParser {
public $parser, $handler;
public $level, $event;
public $buf;
function mono_NativeParser($handler) {
$this->handler = $handler;
$this->parser = xml_parser_create();
xml_parser_set_option($this->parser, XML_OPTION_CASE_FOLDING, 0);
xml_set_object($this->parser, $this);
xml_set_element_handler($this->parser, "begin", "end");
xml_parse($this->parser, "<F>");
$this->level = 0;
}
function begin($parser, $name, $param) {
if (false) $parser = $parser;
$this->event = true;
switch($name) {
case 'X': case 'A': $this->level+=1;
}
$this->handler->begin($name, $param);
}
function end($parser, $name) {
if (false) $parser = $parser;
$this->handler->end($name);
switch($name) {
case 'X': case 'A': $this->level-=1;
}
}
function getData($str) {
return base64_decode($str);
}
function parse() {
do {
$this->event = false;
$buf = $this->buf = $this->handler->read(MONO_RECV_SIZE);
$len = strlen($buf);
if(!xml_parse($this->parser, $buf, $len==0)) {
$this->handler->protocol->handler->shutdownBrokenConnection(
sprintf("protocol error: %s, %s at col %d. Check the back end log for OutOfMemoryErrors.",
$buf,
xml_error_string(xml_get_error_code($this->parser)),
xml_get_current_column_number($this->parser)));
}
} while(!$this->event || $this->level>0);
}
function parserError() {
$this->handler->protocol->handler->shutdownBrokenConnection(
sprintf("protocol error: %s. Check the back end log for details.", $this->buf));
}
}
class mono_Parser {
public $ENABLE_NATIVE = true;
public $DISABLE_SIMPLE= false;
public $parser;
function mono_Parser($handler) {
if($this->ENABLE_NATIVE && function_exists("xml_parser_create")) {
$this->parser = new mono_NativeParser($handler);
$handler->RUNTIME["PARSER"]="NATIVE";
} else {
if($this->DISABLE_SIMPLE) die("no parser");
$this->parser = new mono_SimpleParser($handler);
$handler->RUNTIME["PARSER"]="SIMPLE";
}
}
function parse() {
$this->parser->parse();
}
function getData($str) {
return $this->parser->getData($str);
}
function parserError() {
$this->parser->parserError();
}
}
function mono_getHeader($name, $array) {
if (array_key_exists($name, $array)) return $array[$name];
$name="HTTP_$name"; 
if (array_key_exists($name, $array)) return $array[$name];
return null;
}
function mono_getCompatibilityOption($client) {
static $compatibility = null;
if ($compatibility) return $compatibility;
$compatibility = $client->RUNTIME["PARSER"]=="NATIVE"
? (0103-MONO_PREFER_VALUES)
: (0100+MONO_PREFER_VALUES);
if(is_int(MONO_LOG_LEVEL)) {
$compatibility |= 128 | (7 & MONO_LOG_LEVEL)<<2;
}
$compatibility = chr ($compatibility);
return $compatibility;
}
class mono_SocketChannel {
public $peer, $protocol;
private $channelName, $host;
function mono_SocketChannel($peer, $protocol, $host, $channelName) {
$this->peer = $peer;
$this->protocol = $protocol;
$this->host = $host;
$this->channelName = $channelName;
}
function fwrite($data) {
return fwrite($this->peer, $data);
}
function fread($size) {
return fread($this->peer, $size);
}
function getKeepAlive() {
return "<F p=\"A\" />";
}
function keepAlive() {
$this->fread(10); // <F p="A"/>
}
function shutdownBrokenConnection () {
fclose($this->peer);
}
}
class mono_EmptyPipeChannel {
function open($handler) {
if (false) $handler = $handler;
throw new mono_RuntimeException("protocol error: socket channel names must not start with a slash");
}
function getName() {
return null;
}
function getKeepAlive() {return "";}
function keepAlive() {}
function shutdownBrokenConnection () {}
}
class mono_PipeChannel extends mono_EmptyPipeChannel {
public $peer, $peerr, $peerr_desc, $name;
public $fifo, $fifor;
public $iname, $oname;
function mono_PipeChannel($name) {
$this->name = $name;
$this->iname = $this->name . ".i";
$mask = umask(0);
$this->fifor = posix_mkfifo($this->iname, 0666);
$this->oname = $this->name . ".o";
$this->fifo = posix_mkfifo($this->oname, 0666);
umask($mask);
}
function open($handler) {
if (false) $handler = $handler;
$this->peerr = fopen($this->iname, "r");
$this->peerr_desc = array($this->peerr);
stream_set_blocking($this->peerr, false);
stream_set_timeout($this->peerr, -1);
$this->peer = fopen($this->oname, "w");
unlink($this->iname);
unlink($this->oname);
unlink($this->name);
stream_set_timeout($this->peer, -1);
return $this;
}
function fwrite($data) {
return fwrite($this->peer, $data);
}
function fread($size) {
static $empty = NULL;
stream_select($this->peerr_desc, $empty, $empty, 1677216);
return fread($this->peerr, $size);
}
function getName() {
return $this->name;
}
}
class mono_SocketHandler {
public $protocol, $channel;
function mono_SocketHandler($protocol, $channel) {
$this->protocol = $protocol;
$this->channel = $channel;
}
function write($data) {
return $this->channel->fwrite($data);
}
function read($size) {
return $this->channel->fread($size);
}
function redirect() {}
function getKeepAlive() {
return $this->channel->getKeepAlive();
}
function keepAlive() {
$this->channel->keepAlive();
}
function dieWithBrokenConnection($msg) {
unset($this->protocol->client->protocol);
$error = error_get_last();
die ("${msg} ".$error["message"]);
}
function shutdownBrokenConnection ($msg) {
$this->channel->shutdownBrokenConnection();
$this->dieWithBrokenConnection($msg);
}
}
class mono_SimpleHttpHandler extends mono_SocketHandler {
public $headers;
public $redirect;
public $context, $ssl, $port; 
public $host; 
function createChannel() {
$channelName = mono_getHeader("X_MONOBRIDGE_REDIRECT", $_SERVER);
$context = mono_getHeader("X_MONOBRIDGE_CONTEXT", $_SERVER);
$len = strlen($context);
$len0 = mono_getCompatibilityOption($this->protocol->client); 
$len1 = chr($len&0xFF); $len>>=8;
$len2 = chr($len&0xFF);
$this->protocol->socketHandler=new mono_SocketHandler($this->protocol, $this->channel = $this->getChannel($channelName));
$this->protocol->write("\177${len0}${len1}${len2}${context}");
$this->context = sprintf("X_MONOBRIDGE_CONTEXT: %s\r\n", $context);
$this->protocol->handler = $this->protocol->socketHandler;
$this->protocol->handler->write($this->protocol->client->sendBuffer)
or $this->protocol->handler->shutdownBrokenConnection("x1 Broken local connection handle");
$this->protocol->client->sendBuffer=null;
$this->protocol->handler->read(1)
or $this->protocol->handler->shutdownBrokenConnection("x2 Broken local connection handle");
}
function mono_SimpleHttpHandler($protocol, $ssl, $host, $port) {
$this->protocol = $protocol;
$this->ssl = $ssl;
$this->host = $host;
$this->port = $port;
$this->createChannel();
}
function getCookies() {
$str="";
$first=true;
foreach($_COOKIE as $k => $v) {
$str .= ($first ? "Cookie: $k=$v":"; $k=$v");
$first=false;
}
if(!$first) $str .= "\r\n";
return $str;
}
function getContextFromCgiEnvironment() {
$ctx = mono_getHeader('X_MONOBRIDGE_CONTEXT', $_SERVER);
return $ctx;
}
function getChannelName() {
$name = $this->channel->getName();
return !is_null($name) ? sprintf("X_MONOBRIDGE_CHANNEL: %s\r\n", $name) : null;
}
function getContext() {
$ctx = $this->getContextFromCgiEnvironment();
$context = "";
if($ctx) {
$context = sprintf("X_MONOBRIDGE_CONTEXT: %s\r\n", $ctx);
}
return $context;
}
function getWebAppInternal() {
$context = $this->protocol->webContext;
if(isset($context)) return $context;
return (MONO_SERVLET == "User" &&
array_key_exists('PHP_SELF', $_SERVER) &&
array_key_exists('HTTP_HOST', $_SERVER))
? $_SERVER['PHP_SELF']."monobridge"
: null;
}
function getWebApp() {
$context = $this->getWebAppInternal();
if(is_null($context)) $context = MONO_SERVLET;
if(is_null($context) || $context[0]!="/")
$context = "/MonoBridge/MonoBridge.phpmonobridge";
return $context;
}
function write($data) {
return $this->protocol->socketHandler->write($data);
}
function doSetCookie($key, $val, $path) {
$path=trim($path);
$webapp = $this->getWebAppInternal(); if(!$webapp) $path="/";
setcookie($key, $val, 0, $path);
}
function parseHeaders() {
$this->headers = array();
while (($str = trim(fgets($this->socket, MONO_RECV_SIZE)))) {
if($str[0]=='X') {
if(!strncasecmp("X_MONOBRIDGE_REDIRECT", $str, 21)) {
$this->headers["redirect"]=trim(substr($str, 22));
} else if(!strncasecmp("X_MONOBRIDGE_CONTEXT", $str, 20)) {
$this->headers["context"]=trim(substr($str, 21));
}
} else if($str[0]=='S') { 
if(!strncasecmp("SET-COOKIE", $str, 10)) {
$str=substr($str, 12);
$ar = explode(";", $str);
$cookie = explode("=",$ar[0]);
$path = "";
if(isset($ar[1])) $p=explode("=", $ar[1]);
if(isset($p)) $path=$p[1];
$this->doSetCookie($cookie[0], $cookie[1], $path);
}
} else if($str[0]=='C') { 
if(!strncasecmp("CONTENT-LENGTH", $str, 14)) {
$this->headers["content_length"]=trim(substr($str, 15));
} else if(!strncasecmp("CONNECTION", $str, 10)) {
$this->headers["connection"]=trim(substr($str, 11));
}
}
}
}
function read($size) {
return $this->protocol->socketHandler->read($size);
}
function getChannel($channelName) {
$errstr = null; $errno = null;
if($channelName[0]=='/') return $this->channel->open($this);
$peer = pfsockopen($this->host, $channelName, $errno, $errstr, 30);
if (!$peer) throw new mono_RuntimeException("Could not connect to the context server {$this->host}:{$channelName}. Error message: $errstr ($errno)\n");
stream_set_timeout($peer, -1);
return new mono_SocketChannel($peer, $this->protocol, $this->host, $channelName);
}
function redirect() {}
}
class mono_HttpHandler extends mono_SimpleHttpHandler {
public $socket; 
function createChannel() {
$pipe_dir = $this->protocol->client->RUNTIME['PIPE_DIR'];
if($pipe_dir && ($this->host == "127.0.0.1" || (substr($this->host,0,9) == "localhost")))
$this->channel = new mono_PipeChannel(tempnam($pipe_dir, ".php_mono_bridge"));
else
$this->channel = new mono_EmptyPipeChannel();
}
function close() {
fclose($this->socket);
}
function shutdownBrokenConnection ($msg) {
$this->close();
$this->dieWithBrokenConnection($msg);
}
function open() {
$errno = null; $errstr = null;
$socket = MONO_PERSISTENT_SERVLET_CONNECTIONS ?
pfsockopen("{$this->ssl}{$this->host}", $this->port, $errno, $errstr, 30) :
fsockopen("{$this->ssl}{$this->host}", $this->port, $errno, $errstr, 30);
if (!$socket) throw new mono_ConnectException("Could not connect to the J2EE server {$this->ssl}{$this->host}:{$this->port}. Please start it, for example with the command: \"mono -jar MonoBridge.jar SERVLET:8080 3 MonoBridge.log\" or, if the back end has been compiled to native code, with \"modules/mono SERVLET:8080 3 MonoBridge.log\". Error message: $errstr ($errno)\n");
stream_set_timeout($socket, -1);
return $socket;
}
function mono_HttpHandler($protocol, $ssl, $host, $port) {
parent::mono_SimpleHttpHandler($protocol, $ssl, $host, $port);
$this->socket = $this->open();
}
function write($data) {
$compatibility = mono_getCompatibilityOption($this->protocol->client);
$this->headers = null;
$socket = $this->socket;
$len = 2 + strlen($data);
$webapp = $this->getWebApp();
$cookies = $this->getCookies();
$channel = $this->getChannelName();
$context = $this->getContext();
$redirect = $this->redirect;
$res = "PUT ";
$res .= $webapp;
$res .= MONO_PERSISTENT_SERVLET_CONNECTIONS ? " HTTP/1.1\r\n" : " HTTP/1.0\r\n";
$res .= "Host: {$this->host}:{$this->port}\r\n";
$res .= "Content-Length: "; $res .= $len; $res .= "\r\n";
$res .= $context;
$res .= $cookies;
$res .= $redirect;
if(!is_null($channel)) $res .= $channel;
$res .= "\r\n";
$res .= "\177";
$res .= $compatibility;
$res .= $data;
$count = fwrite($socket, $res) or $this->shutdownBrokenConnection("Broken connection handle");
fflush($socket) or $this->shutdownBrokenConnection("Broken connection handle");
return $count;
}
function read($size) {
if (false) $size = $size;
if(is_null($this->headers)) $this->parseHeaders();
$data = fread($this->socket, $this->headers['content_length']);
return $data;
}
function redirect() {
if(!isset($this->headers["redirect"])) { 
throw new mono_RuntimeException("No Pipe- or SocketContextServer available. See README section \"Security Issues\" for details.");
}
$channelName = $this->headers["redirect"];
$context = $this->headers["context"];
$len = strlen($context);
$len0 = chr(0xFF);
$len1 = chr($len&0xFF); $len>>=8;
$len2 = chr($len&0xFF);
$this->protocol->socketHandler=new mono_SocketHandler($this->protocol, $this->getChannel($channelName));
$this->protocol->write("\177${len0}${len1}${len2}${context}");
$this->context = sprintf("X_MONOBRIDGE_CONTEXT: %s\r\n", $context);
if ((!MONO_PERSISTENT_SERVLET_CONNECTIONS) ||
(array_key_exists("connection", $this->headers)&&
!strncasecmp("close", $this->headers["connection"],5)))
$this->close ();
$this->protocol->handler = $this->protocol->socketHandler;
$this->protocol->handler->write($this->protocol->client->sendBuffer)
or $this->protocol->handler->shutdownBrokenConnection("Broken local connection handle");
$this->protocol->client->sendBuffer=null;
$this->protocol->handler->read(1)
or $this->protocol->handler->shutdownBrokenConnection("Broken local connection handle");
}
}
class mono_Protocol {
public $client;
public $webContext;
public $serverName;
function getOverrideHosts() {
if(array_key_exists('X_MONOBRIDGE_OVERRIDE_HOSTS', $_ENV)) {
$override = $_ENV['X_MONOBRIDGE_OVERRIDE_HOSTS'];
if(!is_null($override) && $override!='/') return $override;
}
return
mono_getHeader('X_MONOBRIDGE_OVERRIDE_HOSTS_REDIRECT', $_SERVER);
}
static function getHost() {
static $host;
if(!isset($host)) {
$hosts = explode(";", MONO_HOSTS);
$host = explode(":", $hosts[0]); 
}
return $host;
}
function createHttpHandler() {
$hostVec = mono_Protocol::getHost();
$host = $hostVec[0];
$port = $hostVec[1];
$overrideHosts = $this->getOverrideHosts();
$ssl = "";
if($overrideHosts) {
$s=$overrideHosts;
if((strlen($s)>2) && ($s[1]==':')) {
if($s[0]=='s')
$ssl="ssl://";
$s = substr($s, 2);
}
$webCtx = strpos($s, "//");
if($webCtx)
$host = substr($s, 0, $webCtx);
else
$host = $s;
$idx = strpos($host, ':');
if($idx) {
if($webCtx)
$port = substr($host, $idx+1, $webCtx);
else
$port = substr($host, $idx+1);
$host = substr($host, 0, $idx);
} else {
$port = "8080";
}
if($webCtx) $webCtx = substr($s, $webCtx+1);
$this->webContext = $webCtx;
}
$this->serverName = "$host:$port";
if ((array_key_exists("X_MONOBRIDGE_REDIRECT", $_SERVER)) ||
(array_key_exists("HTTP_X_MONOBRIDGE_REDIRECT", $_SERVER)))
return new mono_SimpleHttpHandler($this, $ssl, $host, $port);
return new mono_HttpHandler($this, $ssl, $host, $port);
}
function createSimpleHandler($name) {
$channelName = $name;
$errno = null; $errstr = null;
if(!is_string($channelName)) {
$peer = pfsockopen($host="127.0.0.1", $channelName, $errno, $errstr, 30);
} else {
$type = $channelName[0];
if($type=='@' || $type=='/') { 
if($type=='@') $channelName[0]="\0"; 
$peer = pfsockopen($host="unix://${channelName}", null, $errno, $errstr, 30);
$channelName = null;
}
else { 
list($host, $channelName) = explode(":", $channelName);
$peer = pfsockopen($host, $channelName, $errno, $errstr, 30);
}
}
if (!$peer) throw new mono_ConnectException("Could not connect to the server $name. Error message: $errstr ($errno)\n");
stream_set_timeout($peer, -1);
$handler = new mono_SocketHandler($this, new mono_SocketChannel($peer, $this, $host, $channelName));
$compatibility = mono_getCompatibilityOption($this->client);
$this->write("\177$compatibility");
$this->serverName = "127.0.0.1:$channelName";
return $handler;
}
function mono_get_simple_channel() {
if (MONO_HOSTS && (!MONO_SERVLET || (MONO_SERVLET == "Off")) && ($sel=MONO_HOSTS) && ($sel[0]=='@' || ($sel[0]=='/'))) {
$hosts = explode(";", MONO_HOSTS);
return $hosts[0];
}
return null;
}
function createHandler() {
if(!mono_getHeader('X_MONOBRIDGE_OVERRIDE_HOSTS', $_SERVER)&&
((function_exists("mono_get_default_channel")&&($defaultChannel=mono_get_default_channel())) ||
($defaultChannel=$this->mono_get_simple_channel())) ) {
return $this->createSimpleHandler($defaultChannel);
} else {
return $this->createHttpHandler();
}
}
function mono_Protocol ($client) {
$this->client = $client;
$this->handler = $this->createHandler();
}
function redirect() {
$this->handler->redirect();
}
function read($size) {
return $this->handler->read($size);
}
function sendData() {
$this->handler->write($this->client->sendBuffer);
$this->client->sendBuffer=null;
}
function flush() {
if(MONO_DEBUG) {
echo "sending::: "; echo $this->client->sendBuffer; echo "\n";
}
$this->sendData();
}
function getKeepAlive() {
return $this->handler->getKeepAlive();
}
function keepAlive() {
$this->handler->keepAlive();
}
function handle() {
$this->client->handleRequests();
}
function write($data) {
$this->client->sendBuffer.=$data;
}
function finish() {
$this->flush();
$this->handle();
$this->redirect();
}
function referenceBegin($name) {
$this->client->sendBuffer.=$this->client->preparedToSendBuffer;
if(MONO_DEBUG) {
echo "flushed preparedToSendBuffer: ".$this->client->preparedToSendBuffer."\n";
}
$this->client->preparedToSendBuffer=null;
$signature=sprintf("<H p=\"1\" v=\"%s\">", $name);
$this->write($signature);
$signature[6]="2";
$this->client->currentArgumentsFormat = $signature;
}
function referenceEnd() {
$this->client->currentArgumentsFormat.=$format="</H>";
$this->write($format);
$this->finish();
$this->client->currentCacheKey=null;
}
function createObjectBegin($name) {
$this->client->sendBuffer.=$this->client->preparedToSendBuffer;
if(MONO_DEBUG) {
echo "flushed preparedToSendBuffer: ".$this->client->preparedToSendBuffer."\n";
}
$this->client->preparedToSendBuffer=null;
$signature=sprintf("<K p=\"1\" v=\"%s\">", $name);
$this->write($signature);
$signature[6]="2";
$this->client->currentArgumentsFormat = $signature;
}
function createObjectEnd() {
$this->client->currentArgumentsFormat.=$format="</K>";
$this->write($format);
$this->finish();
$this->client->currentCacheKey=null;
}
function propertyAccessBegin($object, $method) {
$this->client->sendBuffer.=$this->client->preparedToSendBuffer;
if(MONO_DEBUG) {
echo "flushed preparedToSendBuffer: ".$this->client->preparedToSendBuffer."\n";
}
$this->client->preparedToSendBuffer=null;
$this->write(sprintf("<G p=\"1\" v=\"%x\" m=\"%s\">", $object, $method));
$this->client->currentArgumentsFormat="<G p=\"2\" v=\"%x\" m=\"${method}\">";
}
function propertyAccessEnd() {
$this->client->currentArgumentsFormat.=$format="</G>";
$this->write($format);
$this->finish();
$this->client->currentCacheKey=null;
}
function invokeBegin($object, $method) {
$this->client->sendBuffer.=$this->client->preparedToSendBuffer;
if(MONO_DEBUG) {
echo "flushed preparedToSendBuffer: ".$this->client->preparedToSendBuffer."\n";
}
$this->client->preparedToSendBuffer=null;
$this->write(sprintf("<Y p=\"1\" v=\"%x\" m=\"%s\">", $object, $method));
$this->client->currentArgumentsFormat="<Y p=\"2\" v=\"%x\" m=\"${method}\">";
}
function invokeEnd() {
$this->client->currentArgumentsFormat.=$format="</Y>";
$this->write($format);
$this->finish();
$this->client->currentCacheKey=null;
}
function resultBegin() {
$this->client->sendBuffer.=$this->client->preparedToSendBuffer;
if(MONO_DEBUG) {
echo "flushed preparedToSendBuffer: ".$this->client->preparedToSendBuffer."\n";
}
$this->client->preparedToSendBuffer=null;
$this->write("<R>");
}
function resultEnd() {
$this->client->currentCacheKey=null;
$this->write("</R>");
$this->flush();
}
function writeString($name) {
$this->client->currentArgumentsFormat.=$format="<S v=\"%s\"/>";
$this->write(sprintf($format, htmlspecialchars($name, ENT_COMPAT)));
}
function writeBoolean($boolean) {
$this->client->currentArgumentsFormat.=$format="<T v=\"%s\"/>";
$this->write(sprintf($format, $boolean));
}
function writeLong($l) {
$this->client->currentArgumentsFormat.="<J v=\"%d\"/>";
if($l<0) {
$this->write(sprintf("<L v=\"%x\" p=\"A\"/>",-$l));
} else {
$this->write(sprintf("<L v=\"%x\" p=\"O\"/>",$l));
}
}
function writeULong($l) {
$this->client->currentArgumentsFormat.=$format="<L v=\"%x\" p=\"O\"/>";
$this->write(sprintf($format,$l));
}
function writeDouble($d) {
$this->client->currentArgumentsFormat.=$format="<D v=\"%.14e\"/>";
$this->write(sprintf($format, $d));
}
function writeObject($object) {
$this->client->currentArgumentsFormat.=$format="<O v=\"%x\"/>";
$this->write(sprintf($format, $object));
}
function writeException($object, $str) {
$this->write(sprintf("<E v=\"%x\" m=\"%s\"/>",$object, htmlspecialchars($str, ENT_COMPAT)));
}
function writeCompositeBegin_a() {
$this->write("<X t=\"A\">");
}
function writeCompositeBegin_h() {
$this->write("<X t=\"H\">");
}
function writeCompositeEnd() {
$this->write("</X>");
}
function writePairBegin_s($key) {
$this->write(sprintf("<P t=\"S\" v=\"%s\">", htmlspecialchars($key, ENT_COMPAT)));
}
function writePairBegin_n($key) {
$this->write(sprintf("<P t=\"N\" v=\"%x\">",$key));
}
function writePairBegin() {
$this->write("<P>");
}
function writePairEnd() {
$this->write("</P>");
}
function writeUnref($object) {
$this->client->sendBuffer.=$this->client->preparedToSendBuffer;
$this->client->preparedToSendBuffer=null;
$this->write(sprintf("<U v=\"%x\"/>", $object));
}
function getServerName() {
return $this->serverName;
}
}
class mono_ParserString {
public $string, $off, $length;
function toString() {
return $this->getString();
}
function getString() {
return substr($this->string, $this->off, $this->length);
}
}
class mono_ParserTag {
public $n, $strings;
function mono_ParserTag() {
$this->strings = array();
$this->n = 0;
}
}
class mono_SimpleParser {
public $SLEN=256; 
public $handler;
public $tag, $buf, $len, $s;
public $type;
function mono_SimpleParser($handler) {
$this->handler = $handler;
$this->tag = array(new mono_ParserTag(), new mono_ParserTag(), new mono_ParserTag());
$this->len = $this->SLEN;
$this->s = str_repeat(" ", $this->SLEN);
$this->type = $this->VOJD;
}
public $BEGIN=0, $KEY=1, $VAL=2, $ENTITY=3, $VOJD=5, $END=6;
public $level=0, $eor=0; public $in_dquote, $eot=false;
public $pos=0, $c=0, $i=0, $i0=0, $e;
function RESET() {
$this->type=$this->VOJD;
$this->level=0;
$this->eor=0;
$this->in_dquote=false;
$this->i=0;
$this->i0=0;
}
function APPEND($c) {
if($this->i>=$this->len-1) {
$this->s=str_repeat($this->s,2);
$this->len*=2;
}
$this->s[$this->i++]=$c;
}
function CALL_BEGIN() {
$pt=&$this->tag[1]->strings;
$st=&$this->tag[2]->strings;
$t=&$this->tag[0]->strings[0];
$name=$t->string[$t->off];
$n = $this->tag[2]->n;
$ar = array();
for($i=0; $i<$n; $i++) {
$ar[$pt[$i]->getString()] = $st[$i]->getString();
}
$this->handler->begin($name, $ar);
}
function CALL_END() {
$t=&$this->tag[0]->strings[0];
$name=$t->string[$t->off];
$this->handler->end($name);
}
function PUSH($t) {
$str = &$this->tag[$t]->strings;
$n = &$this->tag[$t]->n;
$this->s[$this->i]='|';
if(!isset($str[$n])){$h=$this->handler; $str[$n]=$h->createParserString();}
$str[$n]->string=&$this->s;
$str[$n]->off=$this->i0;
$str[$n]->length=$this->i-$this->i0;
++$this->tag[$t]->n;
$this->APPEND('|');
$this->i0=$this->i;
}
function parse() {
while($this->eor==0) {
if($this->c>=$this->pos) {
$this->buf=$this->handler->read(MONO_RECV_SIZE);
if(is_null($this->buf) || strlen($this->buf) == 0)
$this->handler->protocol->handler->shutdownBrokenConnection("protocol error. Check the back end log for OutOfMemoryErrors.");
$this->pos=strlen($this->buf);
if($this->pos==0) break;
$this->c=0;
}
switch(($ch=$this->buf[$this->c]))
{
case '<': if($this->in_dquote) {$this->APPEND($ch); break;}
$this->level+=1;
$this->type=$this->BEGIN;
break;
case '\t': case '\f': case '\n': case '\r': case ' ': if($this->in_dquote) {$this->APPEND($ch); break;}
if($this->type==$this->BEGIN) {
$this->PUSH($this->type);
$this->type = $this->KEY;
}
break;
case '=': if($this->in_dquote) {$this->APPEND($ch); break;}
$this->PUSH($this->type);
$this->type=$this->VAL;
break;
case '/': if($this->in_dquote) {$this->APPEND($ch); break;}
if($this->type==$this->BEGIN) { $this->type=$this->END; $this->level-=1; }
$this->level-=1;
$this->eot=true; 
break;
case '>': if($this->in_dquote) {$this->APPEND($ch); break;}
if($this->type==$this->END){
$this->PUSH($this->BEGIN);
$this->CALL_END();
} else {
if($this->type==$this->VAL) $this->PUSH($this->type);
$this->CALL_BEGIN();
}
$this->tag[0]->n=$this->tag[1]->n=$this->tag[2]->n=0; $this->i0=$this->i=0; 
$this->type=$this->VOJD;
if($this->level==0) $this->eor=1;
break;
case ';':
if($this->type==$this->ENTITY) {
switch ($this->s[$this->e+1]) {
case 'l': $this->s[$this->e]='<'; $this->i=$this->e+1; break; 
case 'g': $this->s[$this->e]='>'; $this->i=$this->e+1; break; 
case 'a': $this->s[$this->e]=($this->s[$this->e+2]=='m'?'&':'\''); $this->i=$this->e+1; break; 
case 'q': $this->s[$this->e]='"'; $this->i=$this->e+1; break; 
default: $this->APPEND($ch);
}
$this->type=$this->VAL; 
} else {
$this->APPEND($ch);
}
break;
case '&':
$this->type = $this->ENTITY;
$this->e=$this->i;
$this->APPEND($ch);
break;
case '"':
$this->in_dquote = !$this->in_dquote;
if(!$this->in_dquote && $this->type==$this->VAL) {
$this->PUSH($this->type);
$this->type = $this->KEY;
}
break;
default:
$this->APPEND($ch);
} 
$this->c+=1;
}
$this->RESET();
}
function getData($str) {
return $str;
}
function parserError() {
$this->handler->protocol->handler->shutdownBrokenConnection(
sprintf("protocol error: %s. Check the back end log for details.", $this->s));
}
}
interface mono_MonoType {};
$mono_initialized = false;
function __monoproxy_Client_getClient() {
static $client;
if(isset($client)) return $client;
if (function_exists("mono_create_client")) $client = mono_create_client();
else {
global $mono_initialized;
$client=new mono_Client();
$mono_initialized = true;
}
return $client;
}
function mono_last_exception_get() {
$client=__monoproxy_Client_getClient();
return $client->getProperty(0, "lastException");
}
function mono_last_exception_clear() {
$client=__monoproxy_Client_getClient();
$client->setProperty(0, "lastException", null);
}
function mono_values_internal($object) {
if(!$object instanceof mono_MonoType) return $object;
$client=__monoproxy_Client_getClient();
return $client->invokeMethod(0, "getValues", array($object));
}
function mono_invoke($object, $method, $args) {
$client=__monoproxy_Client_getClient();
$id = $object?$object->__mono:0;
return $client->invokeMethod($id, $method, $args);
}
function mono_unwrap ($object) {
if(!$object instanceof mono_MonoType) throw new mono_IllegalArgumentException($object);
$client=__monoproxy_Client_getClient();
return $client->globalRef->get($client->invokeMethod(0, "unwrapClosure", array($object)));
}
function mono_values($object) {
return mono_values_internal($object);
}
function mono_reset() {
$client=__monoproxy_Client_getClient();
user_error("Your script has called the privileged procedure \"mono_reset()\" which resets the mono back-end to its initial state. Therefore all mono caches are gone.");
return $client->invokeMethod(0, "reset", array());
}
function mono_inspect_internal($object) {
if(!$object instanceof mono_MonoType) throw new mono_IllegalArgumentException($object);
$client=__monoproxy_Client_getClient();
return $client->invokeMethod(0, "inspect", array($object));
}
function mono_inspect($object) {
return mono_inspect_internal($object);
}
function mono_set_file_encoding($enc) {
$client=__monoproxy_Client_getClient();
return $client->invokeMethod(0, "setFileEncoding", array($enc));
}
function mono_instanceof_internal($ob, $clazz) {
if(!$ob instanceof mono_MonoType) throw new mono_IllegalArgumentException($ob);
if(!$clazz instanceof mono_MonoType) throw new mono_IllegalArgumentException($clazz);
$client=__monoproxy_Client_getClient();
return $client->invokeMethod(0, "instanceOf", array($ob, $clazz));
}
function mono_instanceof($ob, $clazz) {
return mono_instanceof_internal($ob, $clazz);
}
function mono_cast_internal($object, $type) {
if(!$object instanceof mono_MonoType) {
switch($type[0]) {
case 'S': case 's':
return (string)$object;
case 'B': case 'b':
return (boolean)$object;
case 'L': case 'I': case 'l': case 'i':
return (integer)$object;
case 'D': case 'd': case 'F': case 'f':
return (float) $object;
case 'N': case 'n':
return null;
case 'A': case 'a':
return (array)$object;
case 'O': case 'o':
return (object)$object;
}
}
return $object->__cast($type);
}
function mono_cast($object, $type) {
return mono_cast_internal($object, $type);
}
function mono_require($arg) {
$client=__monoproxy_Client_getClient();
return $client->invokeMethod(0, "updateLibraryPath",
array($arg, ini_get("extension_dir"), getcwd(), ini_get("include_path")));
}
function mono_get_lifetime ()
{
$session_max_lifetime=ini_get("session.gc_maxlifetime");
return $session_max_lifetime ? (int)$session_max_lifetime : 1440;
}
function mono_session_array($args) {
$client=__monoproxy_Client_getClient();
if(!isset($args[0])) $args[0]=null;
if(!isset($args[1])) $args[1]=false;
if(!isset($args[2])) {
$args[2] = mono_get_lifetime ();
}
return $client->getSession($args);
}
function mono_session() {
return mono_session_array(func_get_args());
}
function mono_server_name() {
try {
$client=__monoproxy_Client_getClient();
return $client->getServerName();
} catch (mono_ConnectException $ex) {
return null;
}
}
function mono_context() {
$client=__monoproxy_Client_getClient();
return $client->getContext();
}
function mono_closure_array($args) {
if(isset($args[2]) && ((!($args[2] instanceof mono_MonoType))&&!is_array($args[2])))
throw new mono_IllegalArgumentException($args[2]);
$client=__monoproxy_Client_getClient();
$args[0] = isset($args[0]) ? $client->globalRef->add($args[0]) : 0;
$client->protocol->invokeBegin(0, "makeClosure");
$n = count($args);
$client->protocol->writeULong($args[0]); // proper PHP "long" -> Mono 64 bit value conversion
for($i=1; $i<$n; $i++) {
$client->writeArg($args[$i]);
}
$client->protocol->invokeEnd();
$val = $client->getResult();
return $val;
}
function mono_closure() {
return mono_closure_array(func_get_args());
}
function mono_begin_document() {
$client = __monoproxy_Client_getClient();
if (!$client->isAsync) {
$client->invokeMethod(0, "beginDocument", array());
$client->setAsyncHandler();
}
$client->isAsync+=1;
}
function mono_end_document() {
$client = __monoproxy_Client_getClient();
if ($client->isAsync==1) {
$client->setDefaultHandler();
$client->invokeMethod(0, "endDocument", array());
}
if ($client->isAsync > 0) $client->isAsync-=1;
}
class mono_MonoProxy implements mono_MonoType {
public $__serialID, $__mono;
public $__signature;
public $__client;
public $__tempGlobalRef;
function mono_MonoProxy($mono, $signature){
$this->__mono=$mono;
$this->__signature=$signature;
$this->__client = __monoproxy_Client_getClient();
}
function __cast($type) {
return $this->__client->cast($this, $type);
}
function __sleep() {
$args = array($this, mono_get_lifetime());
$this->__serialID = $this->__client->invokeMethod(0, "serialize", $args);
$this->__tempGlobalRef = $this->__client->globalRef;
if(MONO_DEBUG) echo "proxy sleep called for $this->__mono, $this->__signature\n";
return array("__serialID", "__tempGlobalRef");
}
function __wakeup() {
$args = array($this->__serialID, mono_get_lifetime());
if(MONO_DEBUG) echo "proxy wakeup called for $this->__mono, $this->__signature\n";
$this->__client = __monoproxy_Client_getClient();
if($this->__tempGlobalRef)
$this->__client->globalRef = $this->__tempGlobalRef;
$this->__tempGlobalRef = null;
$this->__mono = $this->__client->invokeMethod(0, "deserialize", $args);
}
function __destruct() {
if(isset($this->__client))
$this->__client->unref($this->__mono);
}
function __get($key) {
return $this->__client->getProperty($this->__mono, $key);
}
function __set($key, $val) {
$this->__client->setProperty($this->__mono, $key, $val);
}
function __call($method, $args) {
return $this->__client->invokeMethod($this->__mono, $method, $args);
}
function __toString() {
try {
return $this->__client->invokeMethod(0,"ObjectToString",array($this));
} catch (MonoException $ex) {
trigger_error("Exception in Mono::__toString(): ". (string)$ex, E_USER_WARNING);
return "";
}
}
}
class mono_objectIterator implements Iterator {
private $var;
function mono_ObjectIterator($monoProxy) {
$this->var = mono_cast ($monoProxy, "A");
}
function rewind() {
reset($this->var);
}
function valid() {
return $this->current() !== false;
}
function next() {
return next($this->var);
}
function key() {
return key($this->var);
}
function current() {
return current($this->var);
}
}
class mono_IteratorProxy extends mono_MonoProxy implements IteratorAggregate {
function mono_IteratorProxy($mono, $signature) {
parent::mono_MonoProxy($mono, $signature);
}
function getIterator() {
return new mono_ObjectIterator($this);
}
}
class mono_ArrayProxy extends mono_IteratorProxy implements ArrayAccess {
function mono_ArrayProxy($mono, $signature) {
parent::mono_MonoProxy($mono, $signature);
}
function offsetExists($idx) {
$ar = array($this, $idx);
return $this->__client->invokeMethod(0,"offsetExists", $ar);
}
function offsetGet($idx) {
$ar = array($this, $idx);
return $this->__client->invokeMethod(0,"offsetGet", $ar);
}
function offsetSet($idx, $val) {
$ar = array($this, $idx, $val);
return $this->__client->invokeMethod(0,"offsetSet", $ar);
}
function offsetUnset($idx) {
$ar = array($this, $idx);
return $this->__client->invokeMethod(0,"offsetUnset", $ar);
}
}
class mono_ExceptionProxy extends mono_MonoProxy {
function mono_ExceptionProxy($mono, $signature){
parent::mono_MonoProxy($mono, $signature);
}
function __toExceptionString($trace) {
$args = array($this, $trace);
return $this->__client->invokeMethod(0,"ObjectToString",$args);
}
}
abstract class mono_AbstractMono implements IteratorAggregate,ArrayAccess,mono_MonoType {
public $__client;
public $__delegate;
public $__serialID;
public $__factory;
public $__mono, $__signature;
public $__cancelProxyCreationTag;
function __createDelegate() {
$proxy = $this->__delegate =
$this->__factory->create($this->__mono, $this->__signature);
$this->__mono = $proxy->__mono;
$this->__signature = $proxy->__signature;
}
function __cast($type) {
if(!isset($this->__delegate)) $this->__createDelegate();
return $this->__delegate->__cast($type);
}
function __sleep() {
if(!isset($this->__delegate)) $this->__createDelegate();
$this->__delegate->__sleep();
return array("__delegate");
}
function __wakeup() {
if(!isset($this->__delegate)) $this->__createDelegate();
$this->__delegate->__wakeup();
$this->__mono = $this->__delegate->__mono;
$this->__client = $this->__delegate->__client;
}
function __get($key) {
if(!isset($this->__delegate)) $this->__createDelegate();
return $this->__delegate->__get($key);
}
function __set($key, $val) {
if(!isset($this->__delegate)) $this->__createDelegate();
$this->__delegate->__set($key, $val);
}
function __call($method, $args) {
if(!isset($this->__delegate)) $this->__createDelegate();
return $this->__delegate->__call($method, $args);
}
function __toString() {
if(!isset($this->__delegate)) $this->__createDelegate();
return $this->__delegate->__toString();
}
function getIterator() {
if(!isset($this->__delegate)) $this->__createDelegate();
if(func_num_args()==0) return $this->__delegate->getIterator();
$args = func_get_args(); return $this->__call("getIterator", $args);
}
function offsetExists($idx) {
if(!isset($this->__delegate)) $this->__createDelegate();
if(func_num_args()==1) return $this->__delegate->offsetExists($idx);
$args = func_get_args(); return $this->__call("offsetExists", $args);
}
function offsetGet($idx) {
if(!isset($this->__delegate)) $this->__createDelegate();
if(func_num_args()==1) return $this->__delegate->offsetGet($idx);
$args = func_get_args(); return $this->__call("offsetGet", $args);
}
function offsetSet($idx, $val) {
if(!isset($this->__delegate)) $this->__createDelegate();
if(func_num_args()==2) return $this->__delegate->offsetSet($idx, $val);
$args = func_get_args(); return $this->__call("offsetSet", $args);
}
function offsetUnset($idx) {
if(!isset($this->__delegate)) $this->__createDelegate();
if(func_num_args()==1) return $this->__delegate->offsetUnset($idx);
$args = func_get_args(); return $this->__call("offsetUnset", $args);
}
}
class Mono extends mono_AbstractMono {
function Mono() {
$client = $this->__client = __monoproxy_Client_getClient();
$args = func_get_args();
$name = array_shift($args);
if(is_array($name)) {$args = $name; $name = array_shift($args);}
$name="cli.".$name;
$sig="&{$this->__signature}@{$name}";
$len = count($args);
$args2 = array();
for($i=0; $i<$len; $i++) {
switch(gettype($val = $args[$i])) {
case 'boolean': array_push($args2, $val); $sig.='@b'; break;
case 'integer': array_push($args2, $val); $sig.='@i'; break;
case 'double': array_push($args2, $val); $sig.='@d'; break;
case 'string': array_push($args2, htmlspecialchars($val, ENT_COMPAT)); $sig.='@s'; break;
case 'array':$sig="~INVALID"; break;
case 'object':
if($val instanceof mono_MonoType) {
array_push($args2, $val->__mono);
$sig.="@o{$val->__signature}";
}
else {
$sig="~INVALID";
}
break;
case 'resource': array_push($args2, $val); $sig.='@r'; break;
case 'NULL': array_push($args2, $val); $sig.='@N'; break;
case 'unknown type': array_push($args2, $val); $sig.='@u'; break;
default: throw new mono_IllegalArgumentException($val);
}
}
if(array_key_exists($sig, $client->methodCache)) {
if(MONO_DEBUG) { echo "cache hit for new Mono: $sig\n"; }
$cacheEntry = &$client->methodCache[$sig];
$client->sendBuffer.= $client->preparedToSendBuffer;
if(strlen($client->sendBuffer)>=MONO_SEND_SIZE) {
if($client->protocol->handler->write($client->sendBuffer)<=0)
throw new mono_IllegalStateException("Connection out of sync, check backend log for details.");
$client->sendBuffer=null;
}
$client->preparedToSendBuffer=vsprintf($cacheEntry->fmt, $args2);
if(MONO_DEBUG) {
print_r($args2);
echo "set prepared to send buffer: $client->preparedToSendBuffer, $cacheEntry->fmt, for key: $sig\n";
}
$this->__mono = ++$client->asyncCtx;
if(MONO_DEBUG) {echo "setresult from new Mono cache: object:"; echo sprintf("%x", $client->asyncCtx); echo "\n";}
$this->__factory = $cacheEntry->factory;
$this->__signature = $cacheEntry->signature;
$this->__cancelProxyCreationTag = ++$client->cancelProxyCreationTag;
} else {
if(MONO_DEBUG) { echo "cache miss for new Mono: $sig\n"; }
$client->currentCacheKey = $sig;
$delegate = $this->__delegate = $client->createObject($name, $args);
$this->__mono = $delegate->__mono;
$this->__signature = $delegate->__signature;
}
}
function __destruct() {
if(!isset($this->__client)) return;
$client = $this->__client;
$preparedToSendBuffer = &$client->preparedToSendBuffer;
if($preparedToSendBuffer &&
$client->cancelProxyCreationTag==$this->__cancelProxyCreationTag) {
$preparedToSendBuffer[6]="3";
if(MONO_DEBUG) {
echo "cancel result proxy creation:"; echo $this->__mono; echo " {$client->preparedToSendBuffer}"; echo "\n";
}
$client->sendBuffer.=$preparedToSendBuffer;
$preparedToSendBuffer = null;
$client->asyncCtx -= 1;
} else {
if(!$this->__delegate) { 
if(MONO_DEBUG) {
echo "unref mono:"; echo $this->__mono; echo "\n";
}
$client->unref($this->__mono);
}
}
}
function __call($method, $args) {
$client = $this->__client;
$sig="@{$this->__signature}@$method";
$len = count($args);
$args2=array($this->__mono);
for($i=0; $i<$len; $i++) {
switch(gettype($val = $args[$i])) {
case 'boolean': array_push($args2, $val); $sig.='@b'; break;
case 'integer': array_push($args2, $val); $sig.='@i'; break;
case 'double': array_push($args2, $val); $sig.='@d'; break;
case 'string': array_push($args2, htmlspecialchars($val, ENT_COMPAT)); $sig.='@s'; break;
case 'array':$sig="~INVALID"; break;
case 'object':
if($val instanceof mono_MonoType) {
array_push($args2, $val->__mono);
$sig.="@o{$val->__signature}";
}
else {
$sig="~INVALID";
}
break;
case 'resource': array_push($args2, $val); $sig.='@r'; break;
case 'NULL': array_push($args2, $val); $sig.='@N'; break;
case 'unknown type': array_push($args2, $val); $sig.='@u'; break;
default: throw new mono_IllegalArgumentException($val);
}
}
if(array_key_exists($sig, $client->methodCache)) {
if(MONO_DEBUG) { echo "cache hit for __call: $sig\n"; }
$cacheEntry = &$client->methodCache[$sig];
$client->sendBuffer.=$client->preparedToSendBuffer;
if(strlen($client->sendBuffer)>=MONO_SEND_SIZE) {
if($client->protocol->handler->write($client->sendBuffer)<=0)
throw new mono_IllegalStateException("Out of sync. Check backend log for details.");
$client->sendBuffer=null;
}
$client->preparedToSendBuffer=vsprintf($cacheEntry->fmt, $args2);
if(MONO_DEBUG) {
print_r($args2);
echo "set prepared to send buffer: {$client->preparedToSendBuffer}, {$cacheEntry->fmt}\n";
}
if($cacheEntry->resultVoid) {
$client->cancelProxyCreationTag += 1; 
return null;
} else {
$result = clone($client->cachedMonoPrototype);
$result->__factory = $cacheEntry->factory;
$result->__mono = ++$client->asyncCtx;
if(MONO_DEBUG) {echo "setresult from __call cache: object:"; echo sprintf("%x", $client->asyncCtx); echo "\n";}
$result->__signature = $cacheEntry->signature;
$result->__cancelProxyCreationTag = ++$client->cancelProxyCreationTag;
return $result;
}
} else {
if(MONO_DEBUG) { echo "cache miss for __call: $sig\n"; }
$client->currentCacheKey = $sig;
$retval = parent::__call($method, $args);
return $retval;
}
}
}
class mono_InternalMono extends Mono {
function mono_InternalMono($proxy) {
$this->__delegate = $proxy;
$this->__mono = $proxy->__mono;
$this->__signature = $proxy->__signature;
$this->__client = $proxy->__client;
}
}
class mono_class extends Mono {
function mono_class() {
$this->__client = __monoproxy_Client_getClient();
$args = func_get_args();
$name = array_shift($args);
if(is_array($name)) { $args = $name; $name = array_shift($args); }
$name="cli.".$name;
$delegate = $this->__delegate = $this->__client->referenceObject($name, $args);
$this->__mono = $delegate->__mono;
$this->__signature = $delegate->__signature;
}
}
class MonoClass extends mono_class{}
class mono_exception extends Exception implements mono_MonoType {
public $__serialID, $__mono, $__client;
public $__delegate;
public $__signature;
function mono_exception() {
$this->__client = __monoproxy_Client_getClient();
$args = func_get_args();
$name = array_shift($args);
if(is_array($name)) { $args = $name; $name = array_shift($args); }
if (count($args) >= 1) Exception::__construct($args[0]);
$name="cli.".$name;
$delegate = $this->__delegate = $this->__client->createObject($name, $args);
$this->__mono = $delegate->__mono;
$this->__signature = $delegate->__signature;
}
function __cast($type) {
return $this->__delegate->__cast($type);
}
function __sleep() {
$this->__delegate->__sleep();
return array("__delegate");
}
function __wakeup() {
$this->__delegate->__wakeup();
$this->__mono = $this->__delegate->__mono;
$this->__client = $this->__delegate->__client;
}
function __get($key) {
return $this->__delegate->__get($key);
}
function __set($key, $val) {
$this->__delegate->__set($key, $val);
}
function __call($method, $args) {
return $this->__delegate->__call($method, $args);
}
function __toString() {
return $this->__delegate->__toExceptionString($this->getTraceAsString());
}
}
class MonoException extends mono_exception {}
class mono_InternalException extends MonoException {
function mono_InternalException($proxy, $exception) {
Exception::__construct($exception);
$this->__delegate = $proxy;
$this->__mono = $proxy->__mono;
$this->__signature = $proxy->__signature;
$this->__client = $proxy->__client;
}
}
class mono_MonoProxyProxy extends Mono {
function mono_MonoProxyProxy($client) {
$this->__client = $client;
}
}
if(!function_exists("mono_get_default_channel")){function mono_get_default_channel(){return 9167;}}
}
?>
