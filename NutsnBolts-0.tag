<?xml version='1.0' encoding='UTF-8' standalone='yes' ?>
<tagfile doxygen_version="1.10.0" doxygen_gitid="ebc57c6dd303a980bd19dd74b8b61c8f3f5180ca">
  <compound kind="file">
    <name>SoEvent.h</name>
    <path>/home/runner/work/nutsnbolts/nutsnbolts/lib/NutsnBolts/misc/</path>
    <filename>SoEvent_8h.html</filename>
    <member kind="function">
      <type>SoEvent *</type>
      <name>SoEvent_Clone</name>
      <anchorfile>SoEvent_8h.html</anchorfile>
      <anchor>a6d1b8b5b35398d4bcc5fe9c07fd5b510</anchor>
      <arglist>(const SoEvent *ev)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>SoEvent_Dump</name>
      <anchorfile>SoEvent_8h.html</anchorfile>
      <anchor>a886241db80c86a68683bb728506a4bbe</anchor>
      <arglist>(FILE *stream, const SoEvent *ev)</arglist>
    </member>
    <member kind="function">
      <type>SbBool</type>
      <name>SoEvent_Equals</name>
      <anchorfile>SoEvent_8h.html</anchorfile>
      <anchor>a36298d027129cfed7e198ee7b412149d</anchor>
      <arglist>(const SoEvent *ev1, const SoEvent *ev2)</arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>NbCenterMode</name>
    <filename>classNbCenterMode.html</filename>
    <base>NbNavigationMode</base>
    <member kind="function">
      <type></type>
      <name>NbCenterMode</name>
      <anchorfile>classNbCenterMode.html</anchorfile>
      <anchor>aeb278d4b468a52e9cee7ad324095551f</anchor>
      <arglist>(SbName name)</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual</type>
      <name>~NbCenterMode</name>
      <anchorfile>classNbCenterMode.html</anchorfile>
      <anchor>ac1f71e44d3b5952eee9346f89fa85da9</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function" protection="protected" virtualness="virtual">
      <type>virtual SbBool</type>
      <name>handleEvent</name>
      <anchorfile>classNbCenterMode.html</anchorfile>
      <anchor>ab587e8fdb6c8aebe8d39b8c8e23d81bc</anchor>
      <arglist>(const SoEvent *event, const NbNavigationControl *ctrl)</arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>NbEventNode</name>
    <filename>classNbEventNode.html</filename>
    <base>SoBaseKit</base>
    <member kind="function">
      <type></type>
      <name>NbEventNode</name>
      <anchorfile>classNbEventNode.html</anchorfile>
      <anchor>ac5792203ffc9aa30cb655a12ce236e1b</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual SbBool</type>
      <name>handleEvent</name>
      <anchorfile>classNbEventNode.html</anchorfile>
      <anchor>a3e5ab90fa84378e2e1f77839fcea37e4</anchor>
      <arglist>(const SoEvent *event, SoHandleEventAction *action)</arglist>
    </member>
    <member kind="function" protection="protected" virtualness="virtual">
      <type>virtual</type>
      <name>~NbEventNode</name>
      <anchorfile>classNbEventNode.html</anchorfile>
      <anchor>acfbc6ae8e84e06d1bd251966b3932b86</anchor>
      <arglist>()</arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>NbIdleMode</name>
    <filename>classNbIdleMode.html</filename>
    <base>NbNavigationMode</base>
    <member kind="function">
      <type></type>
      <name>NbIdleMode</name>
      <anchorfile>classNbIdleMode.html</anchorfile>
      <anchor>a8dfd937e4ac112a752437faded128bfa</anchor>
      <arglist>(SbName modename)</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual</type>
      <name>~NbIdleMode</name>
      <anchorfile>classNbIdleMode.html</anchorfile>
      <anchor>a6e2ab0ce91cb35cdccce753e5909b397</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function" protection="protected" virtualness="virtual">
      <type>virtual SbBool</type>
      <name>handleEvent</name>
      <anchorfile>classNbIdleMode.html</anchorfile>
      <anchor>abe42a542f7858b92dda1efc8d38e04da</anchor>
      <arglist>(const SoEvent *event, const NbNavigationControl *ctrl)</arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>NbNamedEvent</name>
    <filename>classNbNamedEvent.html</filename>
    <base>SoEvent</base>
  </compound>
  <compound kind="class">
    <name>NbNavigationControl</name>
    <filename>classNbNavigationControl.html</filename>
    <member kind="function">
      <type></type>
      <name>NbNavigationControl</name>
      <anchorfile>classNbNavigationControl.html</anchorfile>
      <anchor>a031f8cf874c7b51fed71d9e1532ef1a1</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual</type>
      <name>~NbNavigationControl</name>
      <anchorfile>classNbNavigationControl.html</anchorfile>
      <anchor>addfe876225d9a300e819196e451f7214</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>SoCamera *</type>
      <name>getCamera</name>
      <anchorfile>classNbNavigationControl.html</anchorfile>
      <anchor>a371a2a4548303dc77a18bff311cf6731</anchor>
      <arglist>(void) const</arglist>
    </member>
    <member kind="function">
      <type>SoNode *</type>
      <name>getSceneGraph</name>
      <anchorfile>classNbNavigationControl.html</anchorfile>
      <anchor>a01e37718741183e9b7a445f25f6c5bc7</anchor>
      <arglist>(void) const</arglist>
    </member>
    <member kind="function">
      <type>const SbVec3f &amp;</type>
      <name>getUpVector</name>
      <anchorfile>classNbNavigationControl.html</anchorfile>
      <anchor>a420b7dfa9ee73182dfbed97ea09d0407</anchor>
      <arglist>(void) const</arglist>
    </member>
    <member kind="function">
      <type>float</type>
      <name>getViewportAspect</name>
      <anchorfile>classNbNavigationControl.html</anchorfile>
      <anchor>aa7b1e3acdb228b6d09efe5afbc97f450</anchor>
      <arglist>(void) const</arglist>
    </member>
    <member kind="function">
      <type>SbVec2s</type>
      <name>getViewportSize</name>
      <anchorfile>classNbNavigationControl.html</anchorfile>
      <anchor>aa7f79d515b9b914f30d35106b2f2384d</anchor>
      <arglist>(void) const</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>moveCamera</name>
      <anchorfile>classNbNavigationControl.html</anchorfile>
      <anchor>a7320d46695cdddfefc7b1c735f8918d1</anchor>
      <arglist>(const SbVec3f &amp;distance) const</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>moveCamera</name>
      <anchorfile>classNbNavigationControl.html</anchorfile>
      <anchor>acbc0ed11aef8bd00de1b0549375fc2b9</anchor>
      <arglist>(float factor, const SbBool keepfocalpoint=TRUE) const</arglist>
    </member>
    <member kind="function">
      <type>SoPath *</type>
      <name>pick</name>
      <anchorfile>classNbNavigationControl.html</anchorfile>
      <anchor>a872de75be9d62cac3f7a88ef03f8dd3d</anchor>
      <arglist>(SbVec2s pos, SbVec3d &amp;pickpos) const</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>pitchCamera</name>
      <anchorfile>classNbNavigationControl.html</anchorfile>
      <anchor>a27ed86976257b51970f1e81d0e4dbd8d</anchor>
      <arglist>(float angle) const</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>pointDir</name>
      <anchorfile>classNbNavigationControl.html</anchorfile>
      <anchor>a5821c2f26b9dab5d961e65bd445a3a57</anchor>
      <arglist>(const SbVec3f &amp;dir) const</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>reorientCamera</name>
      <anchorfile>classNbNavigationControl.html</anchorfile>
      <anchor>ade93a2cd417195f6a415089d5dc52627</anchor>
      <arglist>(const SbRotation &amp;rot) const</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>reorientCamera</name>
      <anchorfile>classNbNavigationControl.html</anchorfile>
      <anchor>a6d2b11ae08187d053cbdac89582e9310</anchor>
      <arglist>(const SbVec3f &amp;pointat) const</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>resetRoll</name>
      <anchorfile>classNbNavigationControl.html</anchorfile>
      <anchor>a552613366d139c171c29d00b5258c528</anchor>
      <arglist>(void) const</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>restoreCamera</name>
      <anchorfile>classNbNavigationControl.html</anchorfile>
      <anchor>a7e11a5c2cc08a4f5561b384f1cdd0e9f</anchor>
      <arglist>(void) const</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>rollCamera</name>
      <anchorfile>classNbNavigationControl.html</anchorfile>
      <anchor>acf2df865ca05168f54ac5f18f5d9b002</anchor>
      <arglist>(float angle) const</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>saveCamera</name>
      <anchorfile>classNbNavigationControl.html</anchorfile>
      <anchor>a6815c27bc0cfbcb2c17d2b64b2f7ed05</anchor>
      <arglist>(void) const</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>setCamera</name>
      <anchorfile>classNbNavigationControl.html</anchorfile>
      <anchor>afdda187491c0428bdebfe7f872ca1d5f</anchor>
      <arglist>(SoCamera *camera)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>setSceneGraph</name>
      <anchorfile>classNbNavigationControl.html</anchorfile>
      <anchor>a230a5ca78db52b97269852a774ce003f</anchor>
      <arglist>(SoNode *scene)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>setUpVector</name>
      <anchorfile>classNbNavigationControl.html</anchorfile>
      <anchor>ad2828d0454fbe0108f709f3cb987b349</anchor>
      <arglist>(const SbVec3f &amp;up)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>setViewport</name>
      <anchorfile>classNbNavigationControl.html</anchorfile>
      <anchor>af6bdc904559aaade352a2644fbcd112e</anchor>
      <arglist>(const SbViewportRegion &amp;vp)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>viewAll</name>
      <anchorfile>classNbNavigationControl.html</anchorfile>
      <anchor>afe27012a61aeac5bde0b7f9edc9ea503</anchor>
      <arglist>(void) const</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>yawCamera</name>
      <anchorfile>classNbNavigationControl.html</anchorfile>
      <anchor>a9c03361d5e132c0ed93022c1c935c090</anchor>
      <arglist>(float angle) const</arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>NbNavigationMode</name>
    <filename>classNbNavigationMode.html</filename>
    <member kind="function">
      <type></type>
      <name>NbNavigationMode</name>
      <anchorfile>classNbNavigationMode.html</anchorfile>
      <anchor>a0b57db005f92b9b99b93faf29d733c54</anchor>
      <arglist>(SbName modename)</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual</type>
      <name>~NbNavigationMode</name>
      <anchorfile>classNbNavigationMode.html</anchorfile>
      <anchor>af5a5e3fa7cd82d345a445b82a70f84fb</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual void</type>
      <name>abort</name>
      <anchorfile>classNbNavigationMode.html</anchorfile>
      <anchor>a944ef8482ee67f1ce60676c76d32b899</anchor>
      <arglist>(const SoEvent *event, const NbNavigationControl *ctrl)</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual void</type>
      <name>finish</name>
      <anchorfile>classNbNavigationMode.html</anchorfile>
      <anchor>ae9a5f831ab12ca2ed13d1ff0eceeb3ac</anchor>
      <arglist>(const SoEvent *event, const NbNavigationControl *ctrl)</arglist>
    </member>
    <member kind="function">
      <type>NbNavigation1DInputValueFunc *</type>
      <name>get1DValueFunc</name>
      <anchorfile>classNbNavigationMode.html</anchorfile>
      <anchor>a8ba21fad6f5c7982083de2e611bf1460</anchor>
      <arglist>(void **closureptr=NULL) const</arglist>
    </member>
    <member kind="function">
      <type>SbName</type>
      <name>getModeName</name>
      <anchorfile>classNbNavigationMode.html</anchorfile>
      <anchor>ab46d4a9e3d1972cc72a7e5c9c08b5f28</anchor>
      <arglist>(void) const</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual SoNode *</type>
      <name>getSceneGraph</name>
      <anchorfile>classNbNavigationMode.html</anchorfile>
      <anchor>a62c36074a71c2508cc04cfb1910caf7d</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function" virtualness="pure">
      <type>virtual SbBool</type>
      <name>handleEvent</name>
      <anchorfile>classNbNavigationMode.html</anchorfile>
      <anchor>ab8e02846d66056479835c9f06d10b68f</anchor>
      <arglist>(const SoEvent *event, const NbNavigationControl *ctrl)=0</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual void</type>
      <name>init</name>
      <anchorfile>classNbNavigationMode.html</anchorfile>
      <anchor>a19fae2c48e83ef60e895280abeb77b93</anchor>
      <arglist>(const SoEvent *event, const NbNavigationControl *ctrl)</arglist>
    </member>
    <member kind="function">
      <type>SbBool</type>
      <name>processEvent</name>
      <anchorfile>classNbNavigationMode.html</anchorfile>
      <anchor>a68e978b5c7c26a4f59d56bf2c05bd88b</anchor>
      <arglist>(const SoEvent *event, const NbNavigationControl *ctrl)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>set1DValueFunc</name>
      <anchorfile>classNbNavigationMode.html</anchorfile>
      <anchor>a1a49cdeddf0b39d14eaf105a3de62e3b</anchor>
      <arglist>(NbNavigation1DInputValueFunc *func, void *closure)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>initClass</name>
      <anchorfile>classNbNavigationMode.html</anchorfile>
      <anchor>ae5fabf9e01f0725e13612bf7b21297d5</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function" protection="protected">
      <type>SbVec2f</type>
      <name>getCurrentNormalizedPosition</name>
      <anchorfile>classNbNavigationMode.html</anchorfile>
      <anchor>a2e8bd3fca322582290f4c7408e9bee56</anchor>
      <arglist>(const NbNavigationControl *ctrl) const</arglist>
    </member>
    <member kind="function" protection="protected">
      <type>SbVec2s</type>
      <name>getCurrentPosition</name>
      <anchorfile>classNbNavigationMode.html</anchorfile>
      <anchor>a6b9c09689f97396582605bbf25c5ebcb</anchor>
      <arglist>(void) const</arglist>
    </member>
    <member kind="function" protection="protected">
      <type>SbVec2f</type>
      <name>getInitialNormalizedPosition</name>
      <anchorfile>classNbNavigationMode.html</anchorfile>
      <anchor>a143c850e0ac07869bf856ac172d377b3</anchor>
      <arglist>(const NbNavigationControl *ctrl) const</arglist>
    </member>
    <member kind="function" protection="protected">
      <type>SbVec2s</type>
      <name>getInitialPosition</name>
      <anchorfile>classNbNavigationMode.html</anchorfile>
      <anchor>adb06fd89119cb4ccb0b838aad3fc2358</anchor>
      <arglist>(void) const</arglist>
    </member>
    <member kind="function" protection="protected">
      <type>SbVec2f</type>
      <name>getPreviousNormalizedPosition</name>
      <anchorfile>classNbNavigationMode.html</anchorfile>
      <anchor>a1fda5149240304d624d6f8ac16717f51</anchor>
      <arglist>(const NbNavigationControl *ctrl) const</arglist>
    </member>
    <member kind="function" protection="protected">
      <type>SbVec2s</type>
      <name>getPreviousPosition</name>
      <anchorfile>classNbNavigationMode.html</anchorfile>
      <anchor>ae959740cdf30cb8572831e65e5133798</anchor>
      <arglist>(void) const</arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>NbNavigationState</name>
    <filename>classNbNavigationState.html</filename>
    <member kind="function">
      <type></type>
      <name>NbNavigationState</name>
      <anchorfile>classNbNavigationState.html</anchorfile>
      <anchor>a178fdf5205860a48f1dedd53c87a59de</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>~NbNavigationState</name>
      <anchorfile>classNbNavigationState.html</anchorfile>
      <anchor>a81f04ca12b40e015ac8481713a7c4647</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>NbNavigationMode *</type>
      <name>getMode</name>
      <anchorfile>classNbNavigationState.html</anchorfile>
      <anchor>a4c5a823e89b7d74effd02a811de21ddc</anchor>
      <arglist>(void) const</arglist>
    </member>
    <member kind="function">
      <type>const SoEvent *</type>
      <name>getTrigger</name>
      <anchorfile>classNbNavigationState.html</anchorfile>
      <anchor>a0a994e3b6f5662c29f1a5da813a5fa8a</anchor>
      <arglist>(void) const</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>pop</name>
      <anchorfile>classNbNavigationState.html</anchorfile>
      <anchor>a2bf4893b3c1f768e3b919089fadb763c</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>push</name>
      <anchorfile>classNbNavigationState.html</anchorfile>
      <anchor>a4c452a477c38c85fcbd7a63ca9176a7c</anchor>
      <arglist>(NbNavigationMode *mode, const SoEvent *trigger)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>reset</name>
      <anchorfile>classNbNavigationState.html</anchorfile>
      <anchor>a740ac27649d88e10ec297288698b4691</anchor>
      <arglist>(void)</arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>NbNavigationSystem</name>
    <filename>classNbNavigationSystem.html</filename>
    <member kind="enumeration">
      <type></type>
      <name>TransitionType</name>
      <anchorfile>classNbNavigationSystem.html</anchorfile>
      <anchor>abbabeddf32800e651e8dc0650943386e</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>NbNavigationSystem</name>
      <anchorfile>classNbNavigationSystem.html</anchorfile>
      <anchor>a2c89f08aaae205ad93cb5e24a8f8fbef</anchor>
      <arglist>(SbName name)</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>~NbNavigationSystem</name>
      <anchorfile>classNbNavigationSystem.html</anchorfile>
      <anchor>a8a2ea5c113f94185d7d8a7f16a7b3045</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>addMode</name>
      <anchorfile>classNbNavigationSystem.html</anchorfile>
      <anchor>abf8cb57dbf81c11f17866f5139cd30a2</anchor>
      <arglist>(NbNavigationMode *mode)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>addModeChangeCallback</name>
      <anchorfile>classNbNavigationSystem.html</anchorfile>
      <anchor>ac09eac9f59f38c0bcdfa43ddee7edcec</anchor>
      <arglist>(NbNavigationModeChangeCB *cb, void *closure)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>addModeTransition</name>
      <anchorfile>classNbNavigationSystem.html</anchorfile>
      <anchor>a0249422beda3995ed1641845b1ada96d</anchor>
      <arglist>(NbNavigationMode *mode, TransitionType type, const SoEvent *trigger=NULL, const SoEvent *condition=NULL)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>addModeTransition</name>
      <anchorfile>classNbNavigationSystem.html</anchorfile>
      <anchor>ad51f307cfc7fcddd487c2bc0d65ce836</anchor>
      <arglist>(NbNavigationMode *mode1, NbNavigationMode *mode2, TransitionType type, const SoEvent *trigger, const SoEvent *condition=NULL)</arglist>
    </member>
    <member kind="function">
      <type>NbNavigationSystem *</type>
      <name>clone</name>
      <anchorfile>classNbNavigationSystem.html</anchorfile>
      <anchor>a7ee7d8cab3bd59d2270d7e4d672b9bb4</anchor>
      <arglist>(void) const</arglist>
    </member>
    <member kind="function">
      <type>NbNavigationMode *</type>
      <name>getCurrentMode</name>
      <anchorfile>classNbNavigationSystem.html</anchorfile>
      <anchor>ad3100f2c24c28c6bc19d6a34db95c1e3</anchor>
      <arglist>(void) const</arglist>
    </member>
    <member kind="function">
      <type>SbName</type>
      <name>getCurrentModeName</name>
      <anchorfile>classNbNavigationSystem.html</anchorfile>
      <anchor>a0cff207ecc028b72e70ab3c039360502</anchor>
      <arglist>(void) const</arglist>
    </member>
    <member kind="function">
      <type>NbNavigationMode *</type>
      <name>getMode</name>
      <anchorfile>classNbNavigationSystem.html</anchorfile>
      <anchor>a492ebe70ae36fa1dce5bf06035a138de</anchor>
      <arglist>(SbName name) const</arglist>
    </member>
    <member kind="function">
      <type>SbName</type>
      <name>getName</name>
      <anchorfile>classNbNavigationSystem.html</anchorfile>
      <anchor>a6daa58ca5966894f4ce231142f2a47d1</anchor>
      <arglist>(void) const</arglist>
    </member>
    <member kind="function">
      <type>NbNavigationControl *</type>
      <name>getNavigationControl</name>
      <anchorfile>classNbNavigationSystem.html</anchorfile>
      <anchor>a7788f278d9d1b5e6fdf98c69e2e9081f</anchor>
      <arglist>(void) const</arglist>
    </member>
    <member kind="function">
      <type>SbBool</type>
      <name>processEvent</name>
      <anchorfile>classNbNavigationSystem.html</anchorfile>
      <anchor>a1596c180d83891dc8c5a051a33cbf7da</anchor>
      <arglist>(const SoEvent *event)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>removeModeChangeCallback</name>
      <anchorfile>classNbNavigationSystem.html</anchorfile>
      <anchor>ac8a34e19fe00ff8b05074be473455f2f</anchor>
      <arglist>(NbNavigationModeChangeCB *cb, void *closure)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>setCamera</name>
      <anchorfile>classNbNavigationSystem.html</anchorfile>
      <anchor>a013021d039bf21b237032d35c5a51f16</anchor>
      <arglist>(SoCamera *camera)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>setViewport</name>
      <anchorfile>classNbNavigationSystem.html</anchorfile>
      <anchor>a9577b792a7d74a6e27cc87669b0f893d</anchor>
      <arglist>(const SbViewportRegion &amp;viewport)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>viewAll</name>
      <anchorfile>classNbNavigationSystem.html</anchorfile>
      <anchor>ae235f84688fff49c6a43360af6ce1926</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>viewPart</name>
      <anchorfile>classNbNavigationSystem.html</anchorfile>
      <anchor>ab0b0069c91b1f29d4ae8024cb4aec7fa</anchor>
      <arglist>(SoNode *node, const SbVec3f &amp;in, const SbVec3f &amp;up)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>viewPart</name>
      <anchorfile>classNbNavigationSystem.html</anchorfile>
      <anchor>a3a49c3276a061490d96db83502f1347d</anchor>
      <arglist>(SoPath *path, const SbVec3f &amp;in, const SbVec3f &amp;up)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>cleanClass</name>
      <anchorfile>classNbNavigationSystem.html</anchorfile>
      <anchor>a8356243a3a500619df2ed76db22012a9</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static NbNavigationSystem *</type>
      <name>createByName</name>
      <anchorfile>classNbNavigationSystem.html</anchorfile>
      <anchor>abacb00010f0fef9232031d6c26508d9b</anchor>
      <arglist>(SbName name)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static NbNavigationSystem *</type>
      <name>getByName</name>
      <anchorfile>classNbNavigationSystem.html</anchorfile>
      <anchor>acbd6601f5a24122ae476f0dab49e345b</anchor>
      <arglist>(SbName name)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>initClass</name>
      <anchorfile>classNbNavigationSystem.html</anchorfile>
      <anchor>a2fb7b17548f677de9ed736dfb90af423</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static SbBool</type>
      <name>isRegistered</name>
      <anchorfile>classNbNavigationSystem.html</anchorfile>
      <anchor>a4de7e0250059223511f7d080cd758c9f</anchor>
      <arglist>(NbNavigationSystem *system)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static SbBool</type>
      <name>registerSystem</name>
      <anchorfile>classNbNavigationSystem.html</anchorfile>
      <anchor>a7770d6c3854fcb8cd4fa8950db8f14df</anchor>
      <arglist>(NbNavigationSystem *system)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static SbBool</type>
      <name>unregisterSystem</name>
      <anchorfile>classNbNavigationSystem.html</anchorfile>
      <anchor>a32126b6ec62e89042fb5f3ceb23985a0</anchor>
      <arglist>(NbNavigationSystem *system)</arglist>
    </member>
    <member kind="function" protection="protected">
      <type>void</type>
      <name>invokeModeChangeCallbacks</name>
      <anchorfile>classNbNavigationSystem.html</anchorfile>
      <anchor>aa9c150bd8dc28d3ae15c93b4a6907af0</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>NbNavigationModeChangeCB</name>
      <anchorfile>classNbNavigationSystem.html</anchorfile>
      <anchor>a5490346b8eacaeed48ddf44b87aa3450</anchor>
      <arglist>(void *closure, NbNavigationSystem *system)</arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>NbNavigationVehicle</name>
    <filename>classNbNavigationVehicle.html</filename>
  </compound>
  <compound kind="class">
    <name>NbOrthoCenterMode</name>
    <filename>classNbOrthoCenterMode.html</filename>
    <base>NbNavigationMode</base>
    <member kind="function">
      <type></type>
      <name>NbOrthoCenterMode</name>
      <anchorfile>classNbOrthoCenterMode.html</anchorfile>
      <anchor>a4dccc8da65bdf6e3e7905b3bf5424771</anchor>
      <arglist>(SbName name)</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual</type>
      <name>~NbOrthoCenterMode</name>
      <anchorfile>classNbOrthoCenterMode.html</anchorfile>
      <anchor>ab69fde9e385498f0bff83c7343747c8f</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function" protection="protected" virtualness="virtual">
      <type>virtual SbBool</type>
      <name>handleEvent</name>
      <anchorfile>classNbOrthoCenterMode.html</anchorfile>
      <anchor>afe01cf63e683a93d8789225a82365652</anchor>
      <arglist>(const SoEvent *event, const NbNavigationControl *ctrl)</arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>NbPanMode</name>
    <filename>classNbPanMode.html</filename>
    <base>NbNavigationMode</base>
    <member kind="function">
      <type></type>
      <name>NbPanMode</name>
      <anchorfile>classNbPanMode.html</anchorfile>
      <anchor>a43e56d64a050c7764dcfeab37acb4850</anchor>
      <arglist>(SbName name)</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual</type>
      <name>~NbPanMode</name>
      <anchorfile>classNbPanMode.html</anchorfile>
      <anchor>ad77d49457e5d9adf23bb52ec24fce532</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function" protection="protected" virtualness="virtual">
      <type>virtual SbBool</type>
      <name>handleEvent</name>
      <anchorfile>classNbPanMode.html</anchorfile>
      <anchor>aab61997deeedd45a65ccbe45646ff5a0</anchor>
      <arglist>(const SoEvent *event, const NbNavigationControl *ctrl)</arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>NbPickMode</name>
    <filename>classNbPickMode.html</filename>
    <base>NbNavigationMode</base>
    <member kind="function">
      <type></type>
      <name>NbPickMode</name>
      <anchorfile>classNbPickMode.html</anchorfile>
      <anchor>a9cede873dccf06b87e16b34c4afff611</anchor>
      <arglist>(SbName name)</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual</type>
      <name>~NbPickMode</name>
      <anchorfile>classNbPickMode.html</anchorfile>
      <anchor>a432662f4884fee1baf0e5c46d4f291b2</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function" protection="protected" virtualness="virtual">
      <type>virtual SbBool</type>
      <name>handleEvent</name>
      <anchorfile>classNbPickMode.html</anchorfile>
      <anchor>a2807762a850c6fb7a496f5045e054f7a</anchor>
      <arglist>(const SoEvent *event, const NbNavigationControl *ctrl)</arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>NbPitchMode</name>
    <filename>classNbPitchMode.html</filename>
    <base>NbNavigationMode</base>
    <member kind="function">
      <type></type>
      <name>NbPitchMode</name>
      <anchorfile>classNbPitchMode.html</anchorfile>
      <anchor>a87f5a6f4f99009c79eb4fb63d4d5f9a4</anchor>
      <arglist>(SbName name)</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual</type>
      <name>~NbPitchMode</name>
      <anchorfile>classNbPitchMode.html</anchorfile>
      <anchor>ae69068dd0c326b681be614dcf0d0e4d3</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function" protection="protected" virtualness="virtual">
      <type>virtual SbBool</type>
      <name>handleEvent</name>
      <anchorfile>classNbPitchMode.html</anchorfile>
      <anchor>a331e3da043e75ff7fc4c2c021cd04c30</anchor>
      <arglist>(const SoEvent *event, const NbNavigationControl *ctrl)</arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>NbRollMode</name>
    <filename>classNbRollMode.html</filename>
    <base>NbNavigationMode</base>
    <member kind="function" protection="protected" virtualness="virtual">
      <type>virtual SbBool</type>
      <name>handleEvent</name>
      <anchorfile>classNbRollMode.html</anchorfile>
      <anchor>a4f6c2375876c97fb653455a167f169c8</anchor>
      <arglist>(const SoEvent *event, const NbNavigationControl *ctrl)</arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>NbRotateMode</name>
    <filename>classNbRotateMode.html</filename>
    <base>NbNavigationMode</base>
    <member kind="function">
      <type></type>
      <name>NbRotateMode</name>
      <anchorfile>classNbRotateMode.html</anchorfile>
      <anchor>ae01f3c17c6dfd4fbaedebe24bf555cc0</anchor>
      <arglist>(SbName modename)</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual</type>
      <name>~NbRotateMode</name>
      <anchorfile>classNbRotateMode.html</anchorfile>
      <anchor>aeca5d5fb072bb145acf646c41b2db423</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function" protection="protected" virtualness="virtual">
      <type>virtual SbBool</type>
      <name>handleEvent</name>
      <anchorfile>classNbRotateMode.html</anchorfile>
      <anchor>af7df12f0066750c44605b912c19fb9a8</anchor>
      <arglist>(const SoEvent *event, const NbNavigationControl *ctrl)</arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>NbSceneManager</name>
    <filename>classNbSceneManager.html</filename>
    <base>SoSceneManager</base>
    <member kind="enumeration">
      <type></type>
      <name>AutoClippingStrategy</name>
      <anchorfile>classNbSceneManager.html</anchorfile>
      <anchor>ac5def4197b39ad5acbc6d380b968debe</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumeration">
      <type></type>
      <name>NavigationState</name>
      <anchorfile>classNbSceneManager.html</anchorfile>
      <anchor>ab251c7dbe9fa157f9061645bf328b340</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumeration">
      <type></type>
      <name>RenderMode</name>
      <anchorfile>classNbSceneManager.html</anchorfile>
      <anchor>a1e620d5ed75dd688133b726f034e0341</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumeration">
      <type></type>
      <name>StereoMode</name>
      <anchorfile>classNbSceneManager.html</anchorfile>
      <anchor>ab8032b107a183e61437f880e9f33a048</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>NbSceneManager</name>
      <anchorfile>classNbSceneManager.html</anchorfile>
      <anchor>affe7893c7852662414befb8b3afc041d</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual</type>
      <name>~NbSceneManager</name>
      <anchorfile>classNbSceneManager.html</anchorfile>
      <anchor>a8fdcffe088ab51fcb46e549fa40e1723</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual void</type>
      <name>activate</name>
      <anchorfile>classNbSceneManager.html</anchorfile>
      <anchor>a6740517d0d45916015e066112dc874ae</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual void</type>
      <name>deactivate</name>
      <anchorfile>classNbSceneManager.html</anchorfile>
      <anchor>a32cc8bac6aa2853bcfff28b51fd66f62</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual AutoClippingStrategy</type>
      <name>getAutoClipping</name>
      <anchorfile>classNbSceneManager.html</anchorfile>
      <anchor>a5295fd08645786cff956a1ed756109a0</anchor>
      <arglist>(void) const</arglist>
    </member>
    <member kind="function">
      <type>SoCamera *</type>
      <name>getCamera</name>
      <anchorfile>classNbSceneManager.html</anchorfile>
      <anchor>af488154df53be315b1ce82cc2d051278</anchor>
      <arglist>(void) const</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual NavigationState</type>
      <name>getNavigationState</name>
      <anchorfile>classNbSceneManager.html</anchorfile>
      <anchor>ad412b6157a93eefcb54f74a7b0c0d8f0</anchor>
      <arglist>(void) const</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual NbNavigationSystem *</type>
      <name>getNavigationSystem</name>
      <anchorfile>classNbSceneManager.html</anchorfile>
      <anchor>adcb67bde1b95d7ccdd34e165fcf67417</anchor>
      <arglist>(void) const</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual float</type>
      <name>getNearPlaneValue</name>
      <anchorfile>classNbSceneManager.html</anchorfile>
      <anchor>ab13731830194cca0a6002aba34188ba2</anchor>
      <arglist>(void) const</arglist>
    </member>
    <member kind="function">
      <type>RenderMode</type>
      <name>getRenderMode</name>
      <anchorfile>classNbSceneManager.html</anchorfile>
      <anchor>ab1bdc32de54898af74afe84584768da0</anchor>
      <arglist>(void) const</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual SoNode *</type>
      <name>getSceneGraph</name>
      <anchorfile>classNbSceneManager.html</anchorfile>
      <anchor>ae7e132ab4cb1fdfd561ed53b74f532d9</anchor>
      <arglist>(void) const</arglist>
    </member>
    <member kind="function">
      <type>StereoMode</type>
      <name>getStereoMode</name>
      <anchorfile>classNbSceneManager.html</anchorfile>
      <anchor>ab3f9ae59e0a5b0fefbd259166d5cca77</anchor>
      <arglist>(void) const</arglist>
    </member>
    <member kind="function">
      <type>float</type>
      <name>getStereoOffset</name>
      <anchorfile>classNbSceneManager.html</anchorfile>
      <anchor>ad0c2eebeed281f23ca132a611d2b52b5</anchor>
      <arglist>(void) const</arglist>
    </member>
    <member kind="function">
      <type>const SbColor &amp;</type>
      <name>getWireframeOverlayColor</name>
      <anchorfile>classNbSceneManager.html</anchorfile>
      <anchor>a7729c863f2d017f7a7bccad075a9ff21</anchor>
      <arglist>(void) const</arglist>
    </member>
    <member kind="function">
      <type>SbBool</type>
      <name>isTexturesEnabled</name>
      <anchorfile>classNbSceneManager.html</anchorfile>
      <anchor>a0d33321b98dd710a9c30a46b32d78f0a</anchor>
      <arglist>(void) const</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual SbBool</type>
      <name>processEvent</name>
      <anchorfile>classNbSceneManager.html</anchorfile>
      <anchor>a66812776419a69313b32d408889462e9</anchor>
      <arglist>(const SoEvent *const event)</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual void</type>
      <name>setAutoClipping</name>
      <anchorfile>classNbSceneManager.html</anchorfile>
      <anchor>a18809cf15be90e6cb1acde79a9b08948</anchor>
      <arglist>(AutoClippingStrategy strategy)</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual void</type>
      <name>setCamera</name>
      <anchorfile>classNbSceneManager.html</anchorfile>
      <anchor>a3aaad53aa9ffc03294e0bb061467082a</anchor>
      <arglist>(SoCamera *camera)</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual void</type>
      <name>setNavigationState</name>
      <anchorfile>classNbSceneManager.html</anchorfile>
      <anchor>a961f2c2c03dbf14594635a6be3ae7916</anchor>
      <arglist>(NavigationState state)</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual void</type>
      <name>setNavigationSystem</name>
      <anchorfile>classNbSceneManager.html</anchorfile>
      <anchor>a862f80422539f077c929debbf51dbf29</anchor>
      <arglist>(NbNavigationSystem *system)</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual void</type>
      <name>setNearPlaneValue</name>
      <anchorfile>classNbSceneManager.html</anchorfile>
      <anchor>aff3d01207bf9cd112fbe1e0533c6edd5</anchor>
      <arglist>(float value)</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual void</type>
      <name>setRenderMode</name>
      <anchorfile>classNbSceneManager.html</anchorfile>
      <anchor>a8ec10bc730ff8bddb0df9f9b3663e023</anchor>
      <arglist>(RenderMode mode)</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual void</type>
      <name>setSceneGraph</name>
      <anchorfile>classNbSceneManager.html</anchorfile>
      <anchor>a2a68475049174abbbdc0126472c0d9e6</anchor>
      <arglist>(SoNode *const root)</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual void</type>
      <name>setStereoMode</name>
      <anchorfile>classNbSceneManager.html</anchorfile>
      <anchor>aca74e20c1fe82674f1709336887f9d9a</anchor>
      <arglist>(StereoMode mode)</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual void</type>
      <name>setStereoOffset</name>
      <anchorfile>classNbSceneManager.html</anchorfile>
      <anchor>a0a9a0a4dc19486c0bb34082d0027ef31</anchor>
      <arglist>(float offset)</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual void</type>
      <name>setTexturesEnabled</name>
      <anchorfile>classNbSceneManager.html</anchorfile>
      <anchor>a23d4f454e81d6db0832c321d6aaf6826</anchor>
      <arglist>(const SbBool onoff)</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual void</type>
      <name>setWireframeOverlayColor</name>
      <anchorfile>classNbSceneManager.html</anchorfile>
      <anchor>a197c781192227ba3916cb4ade88d88db</anchor>
      <arglist>(const SbColor &amp;color)</arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>NbScriptEvent</name>
    <filename>classNbScriptEvent.html</filename>
    <base>SoNode</base>
    <member kind="function">
      <type></type>
      <name>NbScriptEvent</name>
      <anchorfile>classNbScriptEvent.html</anchorfile>
      <anchor>a78c40d658e6e658423c4c23098c8740f</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>SbBool</type>
      <name>handle</name>
      <anchorfile>classNbScriptEvent.html</anchorfile>
      <anchor>a36f92f8df0c1010e8f2f778b883ebd60</anchor>
      <arglist>(const SoEvent *event, SoHandleEventAction *action)</arglist>
    </member>
    <member kind="function" protection="protected" virtualness="virtual">
      <type>virtual</type>
      <name>~NbScriptEvent</name>
      <anchorfile>classNbScriptEvent.html</anchorfile>
      <anchor>ae52cb4466802c809e7d7ca4709a7023c</anchor>
      <arglist>()</arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>NbViewerNavigationMode</name>
    <filename>classNbViewerNavigationMode.html</filename>
    <base>SoNode</base>
    <member kind="function">
      <type></type>
      <name>NbViewerNavigationMode</name>
      <anchorfile>classNbViewerNavigationMode.html</anchorfile>
      <anchor>a3a07a80058d2cfc49d6f6e29449e8e03</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>initClass</name>
      <anchorfile>classNbViewerNavigationMode.html</anchorfile>
      <anchor>a5d6b87a23ccc38995c633192bca9d53d</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="variable">
      <type>SoSFString</type>
      <name>mode</name>
      <anchorfile>classNbViewerNavigationMode.html</anchorfile>
      <anchor>ab89147242d2344735a868a8a5ece76e0</anchor>
      <arglist></arglist>
    </member>
    <member kind="function" protection="protected" virtualness="virtual">
      <type>virtual</type>
      <name>~NbViewerNavigationMode</name>
      <anchorfile>classNbViewerNavigationMode.html</anchorfile>
      <anchor>a2aa0f1413628f585980bf5dbd12f8307</anchor>
      <arglist>(void)</arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>NbYawMode</name>
    <filename>classNbYawMode.html</filename>
    <base>NbNavigationMode</base>
    <member kind="function" protection="protected" virtualness="virtual">
      <type>virtual SbBool</type>
      <name>handleEvent</name>
      <anchorfile>classNbYawMode.html</anchorfile>
      <anchor>a2a8f6c9abe5f5361309fdb4ad06ac27a</anchor>
      <arglist>(const SoEvent *event, const NbNavigationControl *ctrl)</arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>NbZoomMode</name>
    <filename>classNbZoomMode.html</filename>
    <base>NbNavigationMode</base>
    <member kind="function">
      <type></type>
      <name>NbZoomMode</name>
      <anchorfile>classNbZoomMode.html</anchorfile>
      <anchor>a1d6cad0db24f130fe7e4881eb76f61e7</anchor>
      <arglist>(SbName name)</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual</type>
      <name>~NbZoomMode</name>
      <anchorfile>classNbZoomMode.html</anchorfile>
      <anchor>a098bd8af0346195eb5a17588ea249c2a</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function" protection="protected" virtualness="virtual">
      <type>virtual SbBool</type>
      <name>handleEvent</name>
      <anchorfile>classNbZoomMode.html</anchorfile>
      <anchor>aa480956c2bb0b9fb14b95e6862491ee8</anchor>
      <arglist>(const SoEvent *event, const NbNavigationControl *ctrl)</arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>NutsnBolts</name>
    <filename>classNutsnBolts.html</filename>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>clean</name>
      <anchorfile>classNutsnBolts.html</anchorfile>
      <anchor>a76bb00d5e57924197267cdd151cb75ae</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static int</type>
      <name>getMajorVersion</name>
      <anchorfile>classNutsnBolts.html</anchorfile>
      <anchor>a8eb9dc5eb457885bde6999200895f7e0</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static int</type>
      <name>getMicroVersion</name>
      <anchorfile>classNutsnBolts.html</anchorfile>
      <anchor>aa0fcbc60f2b02b55abe7b9bb5127c597</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static int</type>
      <name>getMinorVersion</name>
      <anchorfile>classNutsnBolts.html</anchorfile>
      <anchor>a950d94b3f4106c40d54229d3ac10439d</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static const char *</type>
      <name>getVersion</name>
      <anchorfile>classNutsnBolts.html</anchorfile>
      <anchor>ad44c9b1ec5403af16d2f25423f16a1a9</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>init</name>
      <anchorfile>classNutsnBolts.html</anchorfile>
      <anchor>a14b306cd7f15bc1dce8c9b7255fceceb</anchor>
      <arglist>(void)</arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>SoBaseKit</name>
    <filename>classSoBaseKit.html</filename>
  </compound>
  <compound kind="class">
    <name>SoEvent</name>
    <filename>classSoEvent.html</filename>
  </compound>
  <compound kind="class">
    <name>SoNode</name>
    <filename>classSoNode.html</filename>
  </compound>
  <compound kind="class">
    <name>SoSceneManager</name>
    <filename>classSoSceneManager.html</filename>
  </compound>
  <compound kind="group">
    <name>actions</name>
    <title>Action Classes</title>
    <filename>group__actions.html</filename>
  </compound>
  <compound kind="group">
    <name>base</name>
    <title>Base Classes</title>
    <filename>group__base.html</filename>
  </compound>
  <compound kind="group">
    <name>bundles</name>
    <title>Bundle Classes</title>
    <filename>group__bundles.html</filename>
  </compound>
  <compound kind="group">
    <name>caches</name>
    <title>Cache Classes</title>
    <filename>group__caches.html</filename>
  </compound>
  <compound kind="group">
    <name>details</name>
    <title>Detail Classes</title>
    <filename>group__details.html</filename>
  </compound>
  <compound kind="group">
    <name>draggers</name>
    <title>Dragger Classes</title>
    <filename>group__draggers.html</filename>
  </compound>
  <compound kind="group">
    <name>elements</name>
    <title>Element Classes</title>
    <filename>group__elements.html</filename>
  </compound>
  <compound kind="group">
    <name>engines</name>
    <title>Engine Classes</title>
    <filename>group__engines.html</filename>
  </compound>
  <compound kind="group">
    <name>errors</name>
    <title>Error Handling Classes</title>
    <filename>group__errors.html</filename>
  </compound>
  <compound kind="group">
    <name>events</name>
    <title>Event Classes</title>
    <filename>group__events.html</filename>
  </compound>
  <compound kind="group">
    <name>fields</name>
    <title>Field Classes</title>
    <filename>group__fields.html</filename>
  </compound>
  <compound kind="group">
    <name>general</name>
    <title>Miscellaneous Classes</title>
    <filename>group__general.html</filename>
  </compound>
  <compound kind="group">
    <name>manips</name>
    <title>Manipulator Classes</title>
    <filename>group__manips.html</filename>
  </compound>
  <compound kind="group">
    <name>navigation</name>
    <title>Navigation Classes</title>
    <filename>group__navigation.html</filename>
    <class kind="class">NbCenterMode</class>
    <class kind="class">NbIdleMode</class>
    <class kind="class">NbNavigationMode</class>
    <class kind="class">NbNavigationState</class>
    <class kind="class">NbNavigationSystem</class>
    <class kind="class">NbNavigationVehicle</class>
    <class kind="class">NbPanMode</class>
    <class kind="class">NbRotateMode</class>
    <class kind="class">NbSceneManager</class>
    <class kind="class">NbViewerNavigationMode</class>
    <class kind="class">NbZoomMode</class>
  </compound>
  <compound kind="group">
    <name>nodekits</name>
    <title>NodeKit Classes</title>
    <filename>group__nodekits.html</filename>
  </compound>
  <compound kind="group">
    <name>nodes</name>
    <title>Node Classes</title>
    <filename>group__nodes.html</filename>
    <class kind="class">NbViewerNavigationMode</class>
  </compound>
  <compound kind="group">
    <name>projectors</name>
    <title>Projector Classes</title>
    <filename>group__projectors.html</filename>
  </compound>
  <compound kind="group">
    <name>sensors</name>
    <title>Sensor Classes</title>
    <filename>group__sensors.html</filename>
  </compound>
  <compound kind="group">
    <name>shaders</name>
    <title>Shader Classes</title>
    <filename>group__shaders.html</filename>
  </compound>
  <compound kind="group">
    <name>soscxml</name>
    <title>Coin specific State Chart XML Classes</title>
    <filename>group__soscxml.html</filename>
  </compound>
  <compound kind="group">
    <name>sound</name>
    <title>3D Sound Support Classes</title>
    <filename>group__sound.html</filename>
  </compound>
  <compound kind="group">
    <name>scxml</name>
    <title>State Chart XML Classes</title>
    <filename>group__scxml.html</filename>
  </compound>
  <compound kind="group">
    <name>threads</name>
    <title>Portable Threads Abstraction Classes</title>
    <filename>group__threads.html</filename>
  </compound>
  <compound kind="group">
    <name>VRMLnodes</name>
    <title>VRML97 Classes</title>
    <filename>group__VRMLnodes.html</filename>
  </compound>
  <compound kind="group">
    <name>hardcopy</name>
    <title>Vectorized Printing Classes</title>
    <filename>group__hardcopy.html</filename>
  </compound>
  <compound kind="group">
    <name>XML</name>
    <title>XML related functions and objects</title>
    <filename>group__XML.html</filename>
  </compound>
  <compound kind="group">
    <name>profiler</name>
    <title>Scene Graph Profiling</title>
    <filename>group__profiler.html</filename>
  </compound>
  <compound kind="group">
    <name>macros</name>
    <title>Miscellaneous Macros</title>
    <filename>group__macros.html</filename>
  </compound>
  <compound kind="group">
    <name>envvars</name>
    <title>Miscellaneous Environment Variables</title>
    <filename>group__envvars.html</filename>
  </compound>
  <compound kind="group">
    <name>misc</name>
    <title>Misc</title>
    <filename>group__misc.html</filename>
    <file>SoEvent.h</file>
    <class kind="class">NbSceneManager</class>
    <class kind="class">NutsnBolts</class>
  </compound>
  <compound kind="group">
    <name>internal</name>
    <title>Internal</title>
    <filename>group__internal.html</filename>
    <file>SoEvent.h</file>
  </compound>
  <compound kind="page">
    <name>navigation</name>
    <title>Navigation</title>
    <filename>navigation.html</filename>
  </compound>
  <compound kind="page">
    <name>index</name>
    <title>Nuts&apos;n&apos;Bolts</title>
    <filename>index.html</filename>
    <docanchor file="index.html" title="Nuts&apos;n&apos;Bolts">md__2home_2runner_2work_2nutsnbolts_2nutsnbolts_2README</docanchor>
  </compound>
</tagfile>
